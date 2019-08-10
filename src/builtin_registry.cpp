#include "builtin_registry.hpp"

namespace Moove {

BuiltinRegistry::Function BuiltinRegistry::findFunction(const std::string& name)const
{
    BuiltinMap::const_iterator pos = m_biMap.find(name);

    return pos != m_biMap.end() ? pos->second : 0;
}

void BuiltinRegistry::registerFunction(const std::string& name, Function func, bool replace)
{
    if(!replace && m_biMap.find(name) != m_biMap.end())
        MOOVE_THROW("builtin '" + name + "' already registered");

    m_biMap[name] = func;
}

void BuiltinRegistry::unregisterFunction(const std::string& name)
{
    BuiltinMap::iterator pos = m_biMap.find(name);

    if(pos != m_biMap.end())
        m_biMap.erase(pos);
}

}       // namespace Moove
