/**
 * \file
 */
#ifndef MOOVE_BUILTIN_REGISTRY_HPP
#define MOOVE_BUITLIN_REGISTRY_HPP

#include "listvar.hpp"
#include "reply.hpp"
#include "string_utils.hpp"

#include <map>
#include <string>

namespace Moove {

class ExecutionState;

class BuiltinRegistry {
public:
    typedef Reply (*Function)(ExecutionState& execState, std::unique_ptr<ListVar> args);

private:
    typedef std::map<std::string, Function, iless> BuiltinMap;

    BuiltinMap m_biMap;

public:
    Function findFunction(const std::string& name)const;

    void registerFunction(const std::string& name, Function func, bool replace = false);

    void unregisterFunction(const std::string& name);
};

}       // namespace Moove

#endif
