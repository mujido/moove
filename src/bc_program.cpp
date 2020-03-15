#include "bc_program.hpp"
#include "except.hpp"

namespace Moove {

BytecodeProgram::BytecodeProgram(std::unique_ptr<ForkVector> forkVect,
                                 LiteralVector& literals,
                                 CodeVector::Word tempBytes,
                                 CodeVector::Word literalBytes,
                                 CodeVector::Word forkBytes) : 
    m_forkVect(std::move(forkVect)), m_tempBytes(tempBytes), m_literalBytes(literalBytes), m_forkBytes(forkBytes)
{
    m_literals.transfer(m_literals.end(), literals);
}

BytecodeProgram::BytecodeProgram(const BytecodeProgram& bc) :
    m_tempBytes(bc.m_tempBytes), m_literalBytes(bc.m_literalBytes), m_forkBytes(bc.m_forkBytes)
{
    m_forkVect.reset(new ForkVector(*bc.m_forkVect));
    m_literals.assign(bc.m_literals);
}

const CodeVector& BytecodeProgram::forkVector(CodeVector::Word i)const
{
    return m_forkVect->at(i);
}

const Variant& BytecodeProgram::literal(CodeVector::Word literalID)const
{
    return m_literals.at(literalID);
}

CodeVector::Word BytecodeProgram::immediateSize(ImmediateValue::Type immType)const
{
    CodeVector::Word size = 0;

    switch(immType) {
        case ImmediateValue::TEMP:
            size = m_tempBytes;
            break;

        case ImmediateValue::LITERAL:
            size = m_literalBytes;
            break;

        case ImmediateValue::FORK:
            size = m_forkBytes;
            break;

        default:
            throw Exception("BytecodeProgram", "immediateSize", "Invalid immediate type");
    };

    return size;
}

BytecodeProgram& BytecodeProgram::operator = (const BytecodeProgram& bc)
{
    if(this != &bc) {
        m_forkVect.reset(new ForkVector(*bc.m_forkVect));
        m_literals.assign(bc.m_literals);

        m_tempBytes = bc.m_tempBytes;
        m_literalBytes = bc.m_literalBytes;
        m_forkBytes = bc.m_forkBytes;
    }

    return *this;
}

DebugBytecodeProgram::DebugBytecodeProgram(std::unique_ptr<ForkVector> forkVect,
                                           LiteralVector& literals,
                                           const SymbolTable& varSymTable,
                                           const SymbolIDMap& varIDMap,
                                           CodeVector::Word tempBytes,
                                           CodeVector::Word literalBytes,
                                           CodeVector::Word forkBytes) :
    BytecodeProgram(std::move(forkVect), literals, tempBytes, literalBytes, forkBytes), m_varSymTable(varSymTable), m_varIDMap(varIDMap)
{
}

Symbol DebugBytecodeProgram::varSymbolByID(CodeVector::Word varID)const
{
    MOOVE_ASSERT(varID < m_varIDMap.size(), "variable ID out of range");
    return m_varIDMap[varID];
}

CodeVector::Word DebugBytecodeProgram::varIDBySymbol(Symbol varSym)const
{
    SymbolIDMap::const_iterator pos = std::find(m_varIDMap.begin(), m_varIDMap.end(), varSym);
    MOOVE_ASSERT(pos != m_varIDMap.end(), "invalid symbol");
    
    return pos - m_varIDMap.begin();
}

}       //namespace Moove
