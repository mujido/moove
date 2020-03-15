//Copyright (c) 2006 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_EXEC_STATE_HPP
#define MOOVE_EXEC_STATE_HPP

#include "builtin_registry.hpp"
#include "op_map.hpp"
#include "string_utils.hpp"
#include "task_frame.hpp"
#include "type_registry.hpp"
#include "task.hpp"

#include "intvar.hpp"
#include "realvar.hpp"
#include "strvar.hpp"
#include "listvar.hpp"

#include <boost/ptr_container/ptr_vector.hpp>

namespace Moove {

class ExecutionState {
private:
    BuiltinRegistry     m_builtinReg;
    TypeRegistry        m_typeReg;
    OperatorMap         m_opMap;
    Task                m_curTask;

    // builtin factory references
    IntVar::Factory*	m_intFactory;
    RealVar::Factory*   m_realFactory;
    StrVar::Factory*    m_strFactory;
    ListVar::Factory*   m_listFactory;

    template<class T>
    typename T::Factory* findTypeFactory(const char *typeName)
    {
        const TypeRegistry::TypeEntry* type = m_typeReg.findType(typeName);
        MOOVE_ASSERT(type, (std::string)"missing required type \"" + typeName + "\"");
        
        typename T::Factory* factory = dynamic_cast<typename T::Factory*>(&type->factory());
        MOOVE_ASSERT(factory, (std::string)"invalid factory for required type \"" + typeName + "\"");
        
        return factory;
    }

public:
    const BuiltinRegistry& builtinRegistry()const
    { return m_builtinReg; }

    BuiltinRegistry& builtinRegistry()
    { return m_builtinReg; }

    const TypeRegistry& typeRegistry()const
    { return m_typeReg; }

    TypeRegistry& typeRegistry()
    { return m_typeReg; }

    const OperatorMap& operatorMap()const
    { return m_opMap; }

    OperatorMap& operatorMap()
    { return m_opMap; }
    
    const Task& currentTask()const
    { return m_curTask; }

    IntVar::Factory& intFactory()const
    { return *m_intFactory; }

    RealVar::Factory& realFactory()const
    { return *m_realFactory; }

    StrVar::Factory& strFactory()const
    { return *m_strFactory; }

    ListVar::Factory& listFactory()const
    { return *m_listFactory; }

    std::unique_ptr<Variant> call(const DebugBytecodeProgram& bc, bool traceFlag = false);

    std::unique_ptr<Variant> call(const DebugBytecodeProgram& bc, Interpreter::VariableDefMap& varDefMap, bool traceFlag = false);
};

}

#endif
