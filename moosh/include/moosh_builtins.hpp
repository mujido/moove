#ifndef MOOSH_BUILTINS_HPP
#define MOOSH_BUILTINS_HPP

#include <exec_state.hpp>
#include <listvar.hpp>
#include <reply.hpp>

Moove::Reply builtin_input(Moove::ExecutionState& execState, std::auto_ptr<Moove::ListVar> args);

Moove::Reply builtin_print(Moove::ExecutionState& execState, std::auto_ptr<Moove::ListVar> args);

Moove::Reply builtin_println(Moove::ExecutionState& execState, std::auto_ptr<Moove::ListVar> args);

Moove::Reply builtin_length(Moove::ExecutionState& execState, std::auto_ptr<Moove::ListVar> args);

Moove::Reply builtin_chr(Moove::ExecutionState& execState, std::auto_ptr<Moove::ListVar> args);

#endif
