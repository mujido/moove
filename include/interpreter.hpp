/**
 * \file
 */
#ifndef MOOVE_INTERPRETER_HPP
#define MOOVE_INTERPRETER_HPP

#include "bc_program.hpp"

#include <iosfwd>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace Moove {

class ExecutionState;

class Interpreter {
public:
    typedef boost::ptr_map<std::string, Variant, iless> VariableDefMap;
    
private:
    typedef boost::ptr_vector<boost::nullable<Variant> > TemporaryValues;

    typedef boost::ptr_vector<Variant> VMStack;

    ExecutionState*                     m_execState;

    std::auto_ptr<BytecodeProgram>      m_bc;
    DebugBytecodeProgram*               m_bcDebug;

    VMStack                             m_stack;
    TemporaryValues                     m_temps;

    const CodeVector*                   m_curVect;
    CodeVector::const_iterator          m_execPos;

    std::auto_ptr<Variant>              m_retVal;

    bool                                m_traceFlag;

    void clearTemps();

    void defineVariables(VariableDefMap& varDefs);

    bool execFinished()const;

    void execBuiltin();

    void lengthList();

    void appendList();

    void indexList();

    void setIndexList();

    void rangeList();

    void spliceList();

    void stepInstruction();

    template<class CharT, class Traits>
    void dumpStack(std::basic_ostream<CharT, Traits>& stream)const
    {
        VMStack::const_iterator stackEnd = m_stack.end();
        for(VMStack::const_iterator stackIter = m_stack.begin(); stackIter != stackEnd; ++stackIter)
            stream << "\t" << stackIter->debugStr() << "\n";
    }

protected:
    template<class VariantType>
    std::auto_ptr<VariantType> popStack()
    {
        MOOVE_ASSERT(!m_stack.empty(), "stack underflow");

        std::auto_ptr<Variant> var(m_stack.release(m_stack.end() - 1).release());
        VariantType* castedVar = dynamic_cast<VariantType*>(var.get());
        MOOVE_ASSERT(castedVar != 0, "invalid type: " + var->factory().regEntry().name());
            
        var.release();
        return std::auto_ptr<VariantType>(castedVar);
    }

    template<class VariantType>
    void pushStack(std::auto_ptr<VariantType> var)
    {
        m_stack.push_back(var.release());
    }

public:
    Interpreter(const DebugBytecodeProgram& bc);

    std::auto_ptr<Variant> run(ExecutionState& execState,
                               VariableDefMap& varDefs, 
                               bool traceFlag = false);
};

template<>
inline std::auto_ptr<Variant> Interpreter::popStack<Variant>()
{
    MOOVE_ASSERT(!m_stack.empty(), "stack underflow");
    
    return std::auto_ptr<Variant>(m_stack.release(m_stack.end() - 1).release());
}

}	// namespace Moove

#endif
