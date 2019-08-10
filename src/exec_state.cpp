#include "exec_state.hpp"

namespace Moove {

std::auto_ptr<Variant> ExecutionState::call(const DebugBytecodeProgram& bc, bool traceFlag)
{
    Interpreter::VariableDefMap varDefMap;

    return call(bc, varDefMap, traceFlag);
}

std::auto_ptr<Variant> ExecutionState::call(const DebugBytecodeProgram& bc, 
                                            Interpreter::VariableDefMap& varDefMap, 
                                            bool traceFlag)
{
    m_intFactory = findTypeFactory<IntVar>("int");
    m_realFactory = findTypeFactory<RealVar>("real");
    m_strFactory = findTypeFactory<StrVar>("str");
    m_listFactory = findTypeFactory<ListVar>("list");

    // create task frame and call code
    m_curTask.push_back(new BytecodeTaskFrame(bc));
    std::auto_ptr<Variant> retVal = m_curTask.back().run(*this, varDefMap, traceFlag);

    // pop task frame and return result
    m_curTask.pop_back();
    return retVal;
}

}       // namespace Moove
