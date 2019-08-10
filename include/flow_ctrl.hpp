//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_FLOW_CTRL_HPP
#define MOOVE_FLOW_CTRL_HPP

#include "codevect.hpp"
#include "except.hpp"
#include "label_linker.hpp"
#include "memory.hpp"
#include "symbol_table.hpp"

#include <deque>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/utility.hpp>

namespace Moove {

class FlowControl : boost::noncopyable {
public:
   enum BlockType {
      LOOP, 
      SWITCH
   };

private:
   struct ControlBlock {
      Symbol      id;
      BlockType   type;
      
      LabelLinker beginLbl;
      LabelLinker continueLbl;
      LabelLinker endLbl;

      ControlBlock(Symbol a_id, BlockType a_type) : id(a_id), type(a_type)
      {}

      ControlBlock(const ControlBlock& blk)
      {}

      bool operator == (const ControlBlock& block)const
      { return id == block.id; }
   };

   typedef boost::ptr_vector<ControlBlock>	ControlBlockList;

   ControlBlockList m_blocks;

   ControlBlock& findCtrlBlock(Symbol id);

public:
   typedef Exception FlowControlError;

   void beginBlock(BlockType type, Symbol id, CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts);

   void setContinueAddr(CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts);

   void endBlock(CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts);

   void addBeginLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts);

   void addContinueLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts);

   void addEndLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts);
};

}   //namespace Moove

#endif   //MOOVE_FLOW_CTRL_HPP
