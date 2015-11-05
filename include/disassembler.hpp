/**
 * \file
 */
#ifndef MOOVE_DISASSEMBLER_HPP
#define MOOVE_DISASSEMBLER_HPP

#include "bc_program.hpp"
#include "opcodes.hpp"

#include <iosfwd>
#include <iomanip>
#include <limits>
#include <string>
#include <cmath>

namespace Moove {

const char* getImmediateTypeStr(ImmediateValue::Type immType);

template<class CharT, class Traits>
void displayBytes(std::basic_ostream<CharT, Traits>& stream, CodeVector::const_iterator pos, CodeVector::size_type n)
{
    // number of characters in a single byte display
    const unsigned BYTE_WIDTH = 2;

    // byte display field needs to be large enough for a 5-byte display of (BYTE_WIDTH + 1 space) characters
    const unsigned BYTE_FIELD_WIDTH = 5 * (BYTE_WIDTH + 1) + 3;
        
    stream.setf(std::ios::right, std::ios::adjustfield);
    stream.setf(std::ios::uppercase);
    
    stream.fill('0');
    for(CodeVector::size_type i = 0; i < n; ++i)
        stream << std::setw(BYTE_WIDTH) << std::hex << static_cast<unsigned>(*pos++) << ' ';
    
    // fill whitespace for remaining width (BYTE_WIDTH characters + 1 space for each byte displayed)
    stream << std::setfill(' ') << std::setw(BYTE_FIELD_WIDTH - n * (BYTE_WIDTH + 1)) << "";
}

template<class CharT, class Traits, class BC>
void displayImmDefault(std::basic_ostream<CharT, Traits>& stream, 
                       const BC& bc, 
                       ImmediateValue::Type immType,
                       CodeVector::Word immID)
{
    if (immType == ImmediateValue::LITERAL) {
        stream << bc.literal(immID).debugStr();
    } else {
        stream << '<' << getImmediateTypeStr(immType) << ": " << std::dec << immID << '>';
    }
}

template<class CharT, class Traits>
void displayImm(std::basic_ostream<CharT, Traits>& stream,
                const BytecodeProgram& bc,
                const OpcodeInfo::Op& op,
                CodeVector::Word immID)
{
    displayImmDefault(stream, bc, op.immediateType(), immID);
}

template<class CharT, class Traits>
void displayImm(std::basic_ostream<CharT, Traits>& stream,
                const DebugBytecodeProgram& bc,
                const OpcodeInfo::Op& op,
                CodeVector::Word immID)
{
    if (op.immediateType() == ImmediateValue::TEMP) {
        Symbol sym = bc.varSymbolByID(immID);

        if (sym) {
            stream << sym.name();
            return;
        }
    }

    displayImmDefault(stream, bc, op.immediateType(), immID);
}
        
template<class CharT, class Traits, class BC>
void disassembleIntern(std::basic_ostream<CharT, Traits>& stream, const BC& bc)
{
    using namespace std;

    const CodeVector& cv = bc.forkVector(0);

    // calculate width of label field = number of digits + 1 empty space
    unsigned labelWidth = static_cast<unsigned>(std::log10(static_cast<float>(cv.size()))) + 2;

    CodeVector::const_iterator end = cv.end();
    for(CodeVector::const_iterator cvByte = cv.begin(); cvByte != end; ) {
        assert(*cvByte < NUM_OPCODES);
        Opcode op = static_cast<Opcode>(*cvByte);

        MOOVE_ASSERT(OpcodeInfo::validOp(op), "invalid opcode");
        const OpcodeInfo::Op& opInfo = OpcodeInfo::getOp(op);

        stream << setfill(' ') << dec << setw(labelWidth) << setiosflags(ios::right) << (cvByte - cv.begin())  << ": ";

        if(opInfo.hasImmediate()) {
            // all of these operators have one immediate value
            CodeVector::Word imm, immSize;

            immSize = (opInfo.immediateType() == ImmediateValue::LABEL ?
                       cv.labelSize() :
                       bc.immediateSize(opInfo.immediateType()));
            imm = cv.unpackWord(cvByte + 1, immSize);

            displayBytes(stream, cvByte, 1 + immSize);
            stream.setf(ios::left, ios::adjustfield);
            stream << setw(20) << opInfo.name();
            displayImm(stream, bc, opInfo, imm);
            stream << '\n';

            cvByte += 1 + immSize;
        } else {
            displayBytes(stream, cvByte, 1);
            stream.setf(ios::left, ios::adjustfield);
            stream << setw(20) << opInfo.name() << '\n';
            ++cvByte;
        }
    }
}

template<class CharT, class Traits>
void disassemble(std::basic_ostream<CharT, Traits>& stream, const BytecodeProgram& bc)
{
    disassembleIntern(stream, bc);
}

template<class CharT, class Traits>
void disassemble(std::basic_ostream<CharT, Traits>& stream, const DebugBytecodeProgram& bc)
{
    disassembleIntern(stream, bc);
}

}       // namespace Moove

#endif  // MOOVE_DISASSEMBLER_HPP
