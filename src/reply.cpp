//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "reply.hpp"

namespace Moove {

Reply::Reply(Type type, std::unique_ptr<Variant> value) : m_type(type), m_value(std::move(value))
{
}

void Reply::reset(Type type, std::unique_ptr<Variant> value)
{
   m_type = type;
   m_value = std::move(value);
}

}   //namespace Moove

