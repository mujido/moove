#include "vm.hpp"
#include "op_package.hpp"

#include <cassert>

namespace Moove {

void VirtualMachine::pushOnStack(boost::shared_ptr<Variant> var)
{
   m_stack.push_front(var);
}

boost::shared_ptr<Variant> VirtualMachine::popOffStack()
{
   assert(!m_stack.empty());
   boost::shared_ptr<Variant> tmp(m_stack[0]);
   m_stack.pop_front();
   return tmp;
}

boost::shared_ptr<Variant> VirtualMachine::stackTop()const
{
   assert(!m_stack.empty());
   return m_stack[0];
}

void VirtualMachine::doUnaryOp(Opcode op)
{
   const OperatorPackage* opPkg = m_opMap->findUnary(m_stack[0]->factory().regEntry());
   assert(op != 0);

   Reply reply = opPkg->doUnaryOp(op, *popOffStack());
   assert(reply.normal());

   m_stack.push_front(reply.value());
}

void VirtualMachine::doBinaryOp(Opcode op)
{
   const OperatorPackage* opPkg = m_opMap->findBinary(m_stack[1]->factory().regEntry(), m_stack[0]->factory().regEntry());
   assert(op != 0);

   boost::shared_ptr<Variant> rhs = popOffStack();
   boost::shared_ptr<Variant> lhs = popOffStack();
   Reply reply = opPkg->doBinaryOp(op, *lhs, *rhs);
   assert(reply.normal());

   m_stack.push_front(reply.value());
}

void VirtualMachine::reset()
{
   m_stack.clear();
}

}	// namespace Moove

