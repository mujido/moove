//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "operators.hpp"

#include "compare.hpp"

#include <map>

namespace Moove {

using namespace std;

struct UnaryOpMask {
   CustomizableOpcode opcode;
   VariantID          type;

   UnaryOpMask(CustomizableOpcode op, VariantID t) : opcode(op), type(t)
   {}
};

bool operator < (const UnaryOpMask& x, const UnaryOpMask& y)
{
   int comp = compare_3way(x.opcode, y.opcode);
   if(comp < 0)
      return true;
   else if(!comp)
      return compare_3way(x.type, y.type) < 0;
   else
      return false;
}

struct BinaryOpMask {
   CustomizableOpcode opcode;
   VariantID          leftType;
   VariantID          rightType;

   BinaryOpMask(CustomizableOpcode op, 
		VariantID left, VariantID right) :
      opcode(op), leftType(left), rightType(right)
   {}
};

bool operator < (const BinaryOpMask& x, const BinaryOpMask& y)
{
   int comp = compare_3way(x.opcode, y.opcode);
   if(comp < 0)
      return true;
   else if(!comp) {
      comp = compare_3way(x.leftType, y.leftType);
      if(comp < 0)
	 return true;
      else if(!comp)
	 return compare_3way(x.rightType, y.rightType) < 0;
   }

   return false;
}
   
struct OperatorMapper::Impl {
   typedef map<UnaryOpMask, const Operator*> UnaryOpMap;
   typedef map<BinaryOpMask, const Operator*> BinaryOpMap;

   UnaryOpMap  unaryOps;
   BinaryOpMap binaryOps;

   Impl()
   {}

   Impl(const Impl& impl) : unaryOps(impl.unaryOps), binaryOps(impl.binaryOps)
   {}
};

Reply Operator::unaryOp(CustomizableOpcode op, const Operand& operand)const
{
   throw UndefinedOperatorMethod("Moove::Operator::unaryOp() called but not defined in child.");
}

Reply Operator::binaryOp(CustomizableOpcode op, 
			 const Operand& left,
			 const Operand& right)const
{
   throw UndefinedOperatorMethod("Moove::Operator::binaryOp() called but not defined in child.");
}

OperatorMapper::OperatorMapper()
{
   m_impl = new Impl;
}

OperatorMapper::OperatorMapper(const OperatorMapper& mapper)
{
   m_impl = new Impl(*mapper.m_impl);
}

OperatorMapper::~OperatorMapper()
{
   delete m_impl;
}

const Operator* OperatorMapper::lookupUnaryOp(CustomizableOpcode opcode,
					      VariantID type)const
{
   Impl::UnaryOpMap::const_iterator pos;

   pos = m_impl->unaryOps.find(UnaryOpMask(opcode, type));
   if(pos != m_impl->unaryOps.end())
      return pos->second;
   else
      return 0;
}

const Operator* 
OperatorMapper::lookupBinaryOp(CustomizableOpcode opcode,
			       VariantID leftType,
			       VariantID rightType)const
{
   Impl::BinaryOpMap::const_iterator pos;

   pos = m_impl->binaryOps.find(BinaryOpMask(opcode, leftType, rightType));
   if(pos != m_impl->binaryOps.end())
      return pos->second;
   else
      return 0;
}

void OperatorMapper::addUnaryOp(CustomizableOpcode opcode,
				VariantID type,
				const Operator& opr,
				bool replace)
{
   if(!replace && lookupUnaryOp(opcode, type))
      throw DuplicateOperatorMask("Attempt to add an operator with duplicate mask in Moove::OperatorMapper::addUnaryOp()");
   else
      m_impl->unaryOps[UnaryOpMask(opcode, type)] = &opr;
}

void OperatorMapper::addBinaryOp(CustomizableOpcode opcode,
				 VariantID leftType,
				 VariantID rightType,
				 const Operator& opr,
				 bool replace)
{
   if(!replace && lookupBinaryOp(opcode, leftType, rightType))
      throw DuplicateOperatorMask("Attempt to add an operator with duplicate mask in Moove::OperatorMapper::addBinaryOp()");
   else
      m_impl->binaryOps[BinaryOpMask(opcode, leftType, rightType)] = &opr;
}
   
bool OperatorMapper::removeUnaryOp(CustomizableOpcode opcode, VariantID type)
{
   Impl::UnaryOpMap::iterator pos;

   pos = m_impl->unaryOps.find(UnaryOpMask(opcode, type));
   if(pos != m_impl->unaryOps.end()) {
      m_impl->unaryOps.erase(pos);
      return true;
   } else
      return false;
}

bool OperatorMapper::removeBinaryOp(CustomizableOpcode opcode,
				    VariantID leftType,
				    VariantID rightType)
{
   Impl::BinaryOpMap::iterator pos;

   pos = m_impl->binaryOps.find(BinaryOpMask(opcode, leftType, rightType));
   if(pos != m_impl->binaryOps.end()) {
      m_impl->binaryOps.erase(pos);
      return true;
   } else
      return false;
}

}   //namespace Moove

