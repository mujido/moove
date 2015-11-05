//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_LABEL_LINKER_HPP
#define MOOVE_LABEL_LINKER_HPP

#include "immediate.hpp"

#include <vector>
#include <boost/utility.hpp>

namespace Moove {

/// Represent a destination label in bytecode for JUMPs and link the referring ImmediateValue label objects
class LabelLinker : boost::noncopyable {
private:
   struct Reference {
      ImmediateValue*		imm;			///< Refers to relevant label ImmediateValue object
      ImmediateValue::Deltas	posImmCounts;		///< Counters for ImmediateValue objects before label's position
   };

   typedef std::vector<Reference> ReferenceList;

   ReferenceList    m_refs;
   CodeVector::Word m_addr;
   bool             m_located;

   ImmediateValue::Deltas	m_destImmCounts;	///< Counters for ImmediateValue objects before label's destination

   void linkLabels()const;

public:
   LabelLinker() : m_located(false)
   {}

   LabelLinker(CodeVector::Word addr) : m_addr(addr), m_located(true)
   {}

   ~LabelLinker()
   { linkLabels(); }

   CodeVector::Word addr()const
   { return m_addr; }

   bool located()const
   { return m_located; }

   void setAddr(CodeVector::Word addr, const ImmediateValue::Deltas& destImmCounts);

   void addLabel(ImmediateValue& label, const ImmediateValue::Deltas& posImmCounts);
};

}    //namespace Moove

#endif    //MOOVE_LABEL_LINKER_HPP

