//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "reply.hpp"

namespace Moove {

Reply::Reply(Type type, boost::shared_ptr<Variant> value) : m_type(type), m_value(value)
{
}

Reply::Reply(Type type, std::auto_ptr<Variant> value) : m_type(type), m_value(boost::shared_ptr<Variant>(value))
{
}

void Reply::reset(Type type, boost::shared_ptr<Variant> value)
{
   m_type = type;
   m_value = value;
}

void Reply::reset(Type type, std::auto_ptr<Variant> value)
{
   m_type = type;
   m_value = boost::shared_ptr<Variant>(value);
}

}   //namespace Moove

