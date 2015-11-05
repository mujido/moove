//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "intvar.hpp"

#include <sstream>

namespace Moove {

DefaultIntVar::Factory DefaultIntVar::s_factory;

std::string IntVar::debugStr()const
{
   std::ostringstream str;

   str << value();
   return str.str();
}

DefaultIntVar* DefaultIntVar::Factory::create()
{
   return new DefaultIntVar;
}

DefaultIntVar* DefaultIntVar::Factory::createValue(value_type value)
{
   return new DefaultIntVar(value);
}

DefaultIntVar::DefaultIntVar()
{
   s_factory.instanceCreated();
}

DefaultIntVar::DefaultIntVar(value_type value) : m_value(value)
{
   s_factory.instanceCreated();
}

DefaultIntVar::DefaultIntVar(const IntVar& var) : m_value(var.value())
{
   s_factory.instanceCreated();
}

DefaultIntVar::~DefaultIntVar()
{
   s_factory.instanceDestroyed();
}

DefaultIntVar* DefaultIntVar::clone()const
{
   return s_factory.createValue(value());
}

}    //namespace Moove


