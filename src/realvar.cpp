//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "realvar.hpp"

#include <limits>
#include <sstream>

namespace Moove {

DefaultRealVar::Factory DefaultRealVar::s_factory;

std::string RealVar::debugStr()const
{
   std::ostringstream str;

   str.precision(std::numeric_limits<value_type>::digits10);
   str << value();
   return str.str();
}

DefaultRealVar* DefaultRealVar::Factory::create()
{
   return new DefaultRealVar;
}

DefaultRealVar* DefaultRealVar::Factory::createValue(const value_type& value)
{
   return new DefaultRealVar(value);
}

DefaultRealVar::DefaultRealVar()
{
   s_factory.instanceCreated();
}

DefaultRealVar::DefaultRealVar(const value_type& value) : m_value(value)
{
   s_factory.instanceCreated();
}

DefaultRealVar::DefaultRealVar(const RealVar& var) : m_value(var.value())
{
   s_factory.instanceCreated();
}

DefaultRealVar::~DefaultRealVar()
{
   s_factory.instanceDestroyed();
}

DefaultRealVar* DefaultRealVar::clone()const
{
   return s_factory.createValue(m_value);
}

}   //namespace Moove
