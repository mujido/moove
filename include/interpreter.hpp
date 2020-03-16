/**
 * \file
 */
#ifndef MOOVE_INTERPRETER_HPP
#define MOOVE_INTERPRETER_HPP

#include "bc_program.hpp"
#include "variant.hpp"

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace Moove {

class ExecutionState;

class Interpreter {
public:
    typedef std::map<std::string, std::unique_ptr<Variant>, iless> VariableDefMap;
    
private:
    typedef std::vector<std::unique_ptr<Variant>> TemporaryValues;
    typedef std::vector<std::unique_ptr<Variant>> VMStack;

    ExecutionState*                     m_execState;

    std::unique_ptr<BytecodeProgram>      m_bc;
    DebugBytecodeProgram*               m_bcDebug;

    VMStack                             m_stack;
    TemporaryValues                     m_temps;

    const CodeVector*                   m_curVect;
    CodeVector::const_iterator          m_execPos;

    std::unique_ptr<Variant>              m_retVal;

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
        for (const auto& stackObj : m_stack)
            stream << "\t" << stackObj->debugStr() << "\n";
    }

protected:
    template<class VariantType>
    std::unique_ptr<VariantType> popStack();

    template<class VariantType>
    void pushStack(std::unique_ptr<VariantType> var)
    {
        m_stack.push_back(std::move(var));
    }

public:
    Interpreter(const DebugBytecodeProgram& bc);

    std::unique_ptr<Variant> run(ExecutionState& execState, VariableDefMap& varDefs, bool traceFlag = false);
};

template<>
inline std::unique_ptr<Variant> Interpreter::popStack<Variant>()
{
    MOOVE_ASSERT(!m_stack.empty(), "stack underflow");
    
    auto tmp = std::move(m_stack.back());
    m_stack.pop_back();
    return tmp;
}

template<class VariantType>
std::unique_ptr<VariantType> Interpreter::popStack()
{
    MOOVE_ASSERT(!m_stack.empty(), "stack underflow");

    auto var = std::move(m_stack.back());
    m_stack.pop_back();
    VariantType* castedVar = dynamic_cast<VariantType*>(var.get());
    MOOVE_ASSERT(castedVar != 0, "invalid type: " + var->factory().regEntry().name());
        
    var.release();
    return std::unique_ptr<VariantType>(castedVar);
}

}	// namespace Moove

#endif
