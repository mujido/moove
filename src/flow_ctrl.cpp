//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "flow_ctrl.hpp"

#include <functional>
#include <memory>

namespace Moove {

FlowControl::ControlBlock& FlowControl::findCtrlBlock(Symbol id)
{
   MOOVE_ASSERT(!m_blocks.empty(), "not in a control block");

   if (id == Symbol()) {
      // return the last block
      return m_blocks.back();
   } 

   ControlBlockList::reverse_iterator blk;
   ControlBlockList::reverse_iterator blkEnd = m_blocks.rend();
   for (blk = m_blocks.rbegin(); blk != blkEnd; ++blk) {
      if (blk->id == id)
	 break;
   }

   MOOVE_ASSERT(blk != blkEnd, "invalid block ID");
   return *blk;
}
   
void FlowControl::beginBlock(BlockType type, Symbol id, CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts)
{
   m_blocks.push_back(new ControlBlock(id, type));
   m_blocks.back().beginLbl.setAddr(addr, prevImmCounts);
}

void FlowControl::setContinueAddr(CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts)
{
   MOOVE_ASSERT(!m_blocks.empty(), "not in a control block");
   MOOVE_ASSERT(m_blocks.back().type == LOOP, "attempt to set 'continue' address on a non-loop control block");
   
   m_blocks.back().continueLbl.setAddr(addr, prevImmCounts);
}

void FlowControl::endBlock(CodeVector::Word addr, const ImmediateValue::Deltas& prevImmCounts)
{
   MOOVE_ASSERT(!m_blocks.empty(), "not in a control block");
   
   m_blocks.back().endLbl.setAddr(addr, prevImmCounts);
   m_blocks.pop_back();
}

void FlowControl::addBeginLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts)
{
   findCtrlBlock(id).beginLbl.addLabel(label, prevImmCounts);
}

void FlowControl::addContinueLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts)
{
   ControlBlock& blk = findCtrlBlock(id);

   MOOVE_ASSERT(blk.type == LOOP, "attempt to reference 'continue' label on a non-loop control block");
   
   blk.continueLbl.addLabel(label, prevImmCounts);
}
      
void FlowControl::addEndLabel(ImmediateValue& label, Symbol id, const ImmediateValue::Deltas& prevImmCounts)
{
   findCtrlBlock(id).endLbl.addLabel(label, prevImmCounts);
}

}   //namespace Moove

   
