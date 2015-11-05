//Copyright (c) 2006 Kurt Stutsman. All rights reserved.
#include "task_frame.hpp"

namespace Moove {

std::auto_ptr<Variant> BytecodeTaskFrame::run(ExecutionState& execState,
                                              Interpreter::VariableDefMap& varDefs,
                                              bool traceFlag)
{
    return m_interp.run(execState, varDefs, traceFlag);
}

}
