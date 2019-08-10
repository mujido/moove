//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "label_linker.hpp"

#include <cassert>
#include <functional>

namespace Moove {

void LabelLinker::linkLabels()const
{
   if (!m_refs.empty()) {
      assert(m_located);
      
      ReferenceList::const_iterator end = m_refs.end();
      for(ReferenceList::const_iterator ref = m_refs.begin(); ref != end; ++ref) {
	 ImmediateValue::Deltas deltas;
	 
	 // store the difference between each ImmediateValue count between the label's destination and its position.
	 std::transform(m_destImmCounts.begin(), m_destImmCounts.end(),
			ref->posImmCounts.begin(), deltas.begin(),
			std::minus<ImmediateValue::Deltas::value_type>());
	 
	 ref->imm->link(m_addr, deltas);
      }
   }
}

void LabelLinker::setAddr(CodeVector::Word addr, const ImmediateValue::Deltas& destImmCounts)
{
   m_addr = addr;
   m_located = true;
   m_destImmCounts = destImmCounts;
}

void LabelLinker::addLabel(ImmediateValue& label, const ImmediateValue::Deltas& posImmCounts)
{
   Reference ref = { &label, posImmCounts };
   m_refs.push_back(ref);
}

}   //namespace Moove
