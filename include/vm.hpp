/**
 * \file
 */
#ifndef MOOVE_VM_HPP
#define MOOVE_VM_HPP

#include "op_map.hpp"
#include "opcodes.hpp"
#include "reply.hpp"

#include <deque>
#include <boost/shared_ptr.hpp>

namespace Moove {

class Variant;

class VirtualMachine {
private:
   typedef std::deque<boost::shared_ptr<Variant> > ArgumentStack;

   ArgumentStack	m_stack;
   const OperatorMap*	m_opMap;

public:
   VirtualMachine(const OperatorMap& opMap) : m_opMap(&opMap)
   {}

   void pushOnStack(boost::shared_ptr<Variant> var);

   boost::shared_ptr<Variant> popOffStack();

   boost::shared_ptr<Variant> stackTop()const;

   void doUnaryOp(Opcode op);

   void doBinaryOp(Opcode op);

   void reset();
};

}	// Moove

#endif	// MOOVE_VM_HPP
