/**
 * \file
 */
#ifndef MOOVE_BC_PROGRAM_HPP
#define MOOVE_BC_PROGRAM_HPP

#include "codevect.hpp"
#include "dynamic_array.hpp"
#include "except.hpp"
#include "immediate.hpp"
#include "symbol_table.hpp"
#include "variant.hpp"

#include <memory>
#include <boost/ptr_container/ptr_vector.hpp>

namespace Moove {

/// Contain all CodeVector objects of a program and its variables and literals.
class BytecodeProgram {
public:
    typedef DynamicArray<CodeVector>	ForkVector;

    typedef boost::ptr_vector<Variant>   LiteralVector;

private:
    std::unique_ptr<ForkVector>	m_forkVect;
    LiteralVector               m_literals;

    CodeVector::Word		m_tempBytes;
    CodeVector::Word		m_literalBytes;
    CodeVector::Word		m_forkBytes;

public:
    BytecodeProgram(std::unique_ptr<ForkVector> forkVect, 
                    LiteralVector& literals,
                    CodeVector::Word tempBytes,
                    CodeVector::Word literalBytes,
                    CodeVector::Word forkBytes);

    BytecodeProgram(const BytecodeProgram& bc);

    virtual ~BytecodeProgram()
    {}

    const CodeVector& forkVector(CodeVector::Word i)const;

    const Variant& literal(CodeVector::Word literalID)const;

    CodeVector::Word immediateSize(ImmediateValue::Type immType)const;

    BytecodeProgram& operator = (const BytecodeProgram& bc);
};

class DebugBytecodeProgram : public BytecodeProgram {
public:
    typedef std::vector<Symbol>		SymbolIDMap;

private:
    SymbolTable		m_varSymTable;
    SymbolIDMap		m_varIDMap;

public:
    DebugBytecodeProgram(std::unique_ptr<ForkVector> forkVect,
			 LiteralVector& literals,
			 const SymbolTable& varSymTable,
			 const SymbolIDMap& varIDMap,
			 CodeVector::Word tempBytes,
			 CodeVector::Word literalBytes,
			 CodeVector::Word forkBytes);

    const SymbolTable& varSymTable()const
    { return m_varSymTable; }

    Symbol varSymbolByID(CodeVector::Word varID)const;

    CodeVector::Word varIDBySymbol(Symbol varSym)const;
};

}	//namespace Moove

#endif	// MOOVE_BC_PROGRAM_HPP
