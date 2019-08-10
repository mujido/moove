//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "operators.hpp"
#include "intvar.hpp"
#include "realvar.hpp"
#include "strvar.hpp"
#include "type_registry.hpp"

#include <iostream>
#include <vector>

using namespace Moove;
using namespace std;

template<class Left, class Right, class Result>
struct Add : public Operator {
   Reply binaryOp(CustomizableOpcode, 
		  const Operand& left, const Operand& right)const
   {
      const Left& leftVar = dynamic_cast<const Left&>(*left.value);
      const Right& rightVar = dynamic_cast<const Right&>(*right.value);
      Reply reply;

      reply.reset(Reply::NORMAL, 
		  new Result(leftVar.value() + rightVar.value()));
      return reply;
   }
};

static const Add<IntVar, IntVar, DefaultIntVar> AddIntInt;
static const Add<IntVar, RealVar, DefaultRealVar> AddIntReal;
static const Add<RealVar, IntVar, DefaultRealVar> AddRealInt;
static const Add<RealVar, RealVar, DefaultRealVar> AddRealReal;
static const Add<StrVar, StrVar, DefaultStrVar> AddStrStr;

static DefaultIntFactory intFactory;
static DefaultRealFactory realFactory;
static DefaultStrFactory strFactory;

int main()
{
   try {
      OperatorMapper mapper;
      TypeRegistry registry;
      
      const VariantID intID = intFactory.productID();
      const VariantID realID = realFactory.productID();
      const VariantID strID = strFactory.productID();
      
      registry.registerType("int", intFactory);
      registry.registerType("real", realFactory);
      registry.registerType("str", strFactory);
      
      mapper.addBinaryOp(OP_ADD, intID, intID, AddIntInt);
      mapper.addBinaryOp(OP_ADD, intID, realID, AddIntReal);
      mapper.addBinaryOp(OP_ADD, realID, intID, AddRealInt);
      mapper.addBinaryOp(OP_ADD, realID, realID, AddRealReal);
      mapper.addBinaryOp(OP_ADD, strID, strID, AddStrStr);
      
      typedef vector<Variant*> VariantVector;
      
      VariantVector leftVars, rightVars;
      
      leftVars.push_back(intFactory.createInt(1));
      leftVars.push_back(realFactory.createReal(1.234));
      leftVars.push_back(strFactory.createStr("abc"));
      
      rightVars.push_back(intFactory.createInt(2));
      rightVars.push_back(realFactory.createReal(3.456));
      rightVars.push_back(strFactory.createStr("xyz"));
      
      for(VariantVector::const_iterator i = leftVars.begin();
	  i != leftVars.end(); ++i) {
	 for(VariantVector::const_iterator j = rightVars.begin();
	     j != rightVars.end(); ++j) {
	    Reply result;
	    const Operator* op;

	    const TypeRegistry::Info* iInfo = registry.lookupType((*i)->id());
	    const TypeRegistry::Info* jInfo = registry.lookupType((*j)->id());

	    op = mapper.lookupBinaryOp(OP_ADD, iInfo->id(), jInfo->id());
	    if(!op)
	       cerr << "Lookup failed for " << iInfo->name()
		    << " + " << jInfo->name() << endl;
	    else {
	       result = op->binaryOp(OP_ADD,
				     Operator::Operand(iInfo->id(), *i),
				     Operator::Operand(jInfo->id(), *j));
	       
	       cout << iInfo->name() << " + " << jInfo->name() << " result: ";
	       
	       if(result.value()->id() == intID)
		  cout << dynamic_cast<const IntVar&>(*result.value()).value();
	       else if(result.value()->id() == realID)
		  cout << dynamic_cast<const RealVar&>(*result.value()).value();
	       else if(result.value()->id() == strID)
		  cout << dynamic_cast<const StrVar&>(*result.value()).value();
	       else {
		  cerr << "Unexpected resultant type" << endl;
		  return 1;
	       }
	       
	       cout << endl;
	    }
	 }
      }
   } catch(exception& e) {
      cerr << "Exception thrown: " << e.what() << endl;
      return 1;
   }

   return 0;
}
