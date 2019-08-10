//Copyright (c) 2005 Kurt Stutsman. All rights reserved.
#include "immediate.hpp"

#include <algorithm>

namespace Moove {

ImmediateValue::ImmediateValue(CodeVector::Word pos, Type type, CodeVector::Word value) : 
   m_pos(pos), m_type(type), m_value(value)
{
   std::fill(m_immDeltas.begin(), m_immDeltas.end(), 0);
}

void ImmediateValue::link(CodeVector::Word addr, const Deltas& immDeltas)
{
   m_value = addr;
   m_immDeltas = immDeltas;
}

}	// namespace Moove
