//Copyright (c) 2005 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_TASK_FRAME_HPP
#define MOOVE_TASK_FRAME_HPP

#include "bc_program.hpp"
#include "interpreter.hpp"
#include "variant.hpp"

#include <boost/ptr_container/ptr_vector.hpp>

namespace Moove {

class ExecutionState;

struct TaskFrame {
    virtual std::unique_ptr<Variant> run(ExecutionState& execState,
                                       Interpreter::VariableDefMap& varDefMap, 
                                       bool traceFlag = false) = 0;
};
    
class BytecodeTaskFrame : public TaskFrame {
private:
    Interpreter m_interp;

public:
    BytecodeTaskFrame(const DebugBytecodeProgram& bc) : m_interp(bc)
    {}

    std::unique_ptr<Variant> run(ExecutionState& execState,
                               Interpreter::VariableDefMap& varDefs, 
                               bool traceFlag = false);
};

}       // namespace Moove

#endif
