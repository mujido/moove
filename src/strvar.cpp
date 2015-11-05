//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "strvar.hpp"

#include <sstream>

namespace Moove {

DefaultStrVar::Factory DefaultStrVar::s_factory;

std::string StrVar::debugStr()const
{
   return "\"" + value() + "\"";
}

DefaultStrVar* DefaultStrVar::Factory::create()
{
   return new DefaultStrVar;
}

DefaultStrVar* DefaultStrVar::Factory::createValue(const value_type& value)
{
   return new DefaultStrVar(value);
}

DefaultStrVar* DefaultStrVar::clone()const
{
   return s_factory.createValue(m_value);
}

void DefaultStrVar::setValue(const value_type& value)
{
   m_value = value;
}

}    //namespace Moove

