#include "type_registry.hpp"
#include "intvar.hpp"
#include "realvar.hpp"
#include "strvar.hpp"
#include "op_map.hpp"
#include "op_package.hpp"

#include <cassert>
#include <iostream>
#include <memory>

using namespace std;
using namespace Moove;

struct IntIntOpPkg : public OperatorPackage {
   Reply doBinaryOp(Opcode op, const Variant& left, const Variant& right)const;
};

struct RealRealOpPkg : public OperatorPackage {
   Reply doBinaryOp(Opcode op, const Variant& left, const Variant& right)const;
};

class IntRealOpPkg : public OperatorPackage {
protected:
   static double upcastToReal(const Variant& var);

   Reply doBinaryOp(Opcode op, const Variant& left, const Variant& right)const;
};

namespace {

TypeRegistry typeReg;

}
      
Reply IntIntOpPkg::doBinaryOp(Opcode op, const Variant& left, const Variant& right)const
{
   std::auto_ptr<IntVar> resultVar(dynamic_cast<IntVar*>(left.clone()));

   if(op == OP_ADD) {
      resultVar->setValue(resultVar->value() + dynamic_cast<const IntVar&>(right).value());
   } else
      return OperatorPackage::doBinaryOp(op, left, right);

   Reply reply(Reply::NORMAL, std::auto_ptr<Variant>(resultVar));
}

Reply RealRealOpPkg::doBinaryOp(Opcode op, const Variant& left, const Variant& right)const
{
   std::auto_ptr<RealVar> resultVar(dynamic_cast<RealVar*>(left.clone()));

   if(op == OP_ADD)
      resultVar->setValue(resultVar->value() + dynamic_cast<const RealVar&>(right).value());
   else {
      return OperatorPackage::doBinaryOp(op, left, right);
   }

   return Reply(Reply::NORMAL, std::auto_ptr<Variant>(resultVar));
}

double IntRealOpPkg::upcastToReal(const Variant& var)
{
   if(const IntVar* intVar = dynamic_cast<const IntVar*>(&var)) {
      return intVar->value();
   } 

   return dynamic_cast<const RealVar&>(var).value();
}

Reply IntRealOpPkg::doBinaryOp(Opcode op, const Variant& left, const Variant& right)const
{
   const TypeRegistry::TypeEntry* realType = typeReg.findType("real");
   assert(realType != 0);

   std::auto_ptr<RealVar> resultVar(dynamic_cast<RealVar*>(realType->factory().create()));

   if(op == OP_ADD)
      resultVar->setValue(upcastToReal(left) + upcastToReal(right));
   else
      return OperatorPackage::doBinaryOp(op, left, right);

   return Reply(Reply::NORMAL, std::auto_ptr<Variant>(resultVar));
}

namespace {

IntIntOpPkg	pkgIntInt;
RealRealOpPkg	pkgRealReal;
IntRealOpPkg	pkgIntReal;

}

int main()
{
   try {
      OperatorMap opMap;

      typeReg.registerType("int", DefaultIntVar::classFactory());
      typeReg.registerType("real", DefaultRealVar::classFactory());
      typeReg.registerType("str", DefaultStrVar::classFactory());

      const TypeRegistry::TypeEntry*	intType = typeReg.findType("int");
      const TypeRegistry::TypeEntry*	realType = typeReg.findType("real");
      const TypeRegistry::TypeEntry*	strType = typeReg.findType("str");

      if(!intType || !realType || !strType) {
	 cerr << "Error locating type" << endl;
	 return 1;
      }

      opMap.reserve(typeReg.typeCount());
      opMap.registerBinary(*intType, *intType, pkgIntInt);
      opMap.registerBinary(*realType, *realType, pkgRealReal);
      opMap.registerBinary(*intType, *realType, pkgIntReal);
      opMap.registerBinary(*realType, *intType, pkgIntReal);

      cout << "Beginning loop\n";

      // profiling loop
      Reply reply;
      std::auto_ptr<IntVar>	vInt;
      std::auto_ptr<RealVar>	vReal;

      for(int i = 0; i < 300000; ++i) {
	 vInt.reset(dynamic_cast<IntVar*>(intType->factory().create()));
	 vReal.reset(dynamic_cast<RealVar*>(realType->factory().create()));

	 vInt->setValue(123);
	 vReal->setValue(456.789);

	 const OperatorPackage* opPkg = opMap.findBinary(vInt->factory().regEntry(), vReal->factory().regEntry());
	 if(!opPkg) {
	    cerr << "Error locating operator package" << endl;
	    return 1;
	 }

	 reply = opPkg->doBinaryOp(OP_ADD, *vInt, *vReal);
	 if(!reply.normal()) {
	    cerr << "Error occured while executing add operation" << endl;
	    return 1;
	 }
      }

      const RealVar& vSum = dynamic_cast<const RealVar&>(*reply.value());
      cout << "Sum of " << vInt->value() << " + " << vReal->value() << " = " << vSum.value() << "\n";

      std::auto_ptr<StrVar>	vStr1(dynamic_cast<StrVar*>(strType->factory().create()));
      std::auto_ptr<StrVar>	vStr2(dynamic_cast<StrVar*>(vStr1->clone()));

      vStr1->setValue("All I want to do ");
      vStr2->setValue("is have some fun");

      cout << "str1 = \"" << vStr1->value() << "\"\n";
      cout << "str2 = \"" << vStr2->value() << "\"\n";

      // release a str to check decrementing of instance count
      vStr2.reset();
      
      // loop through all type registries and print instance count
      const TypeRegistry::const_iterator end = typeReg.end();
      for(TypeRegistry::const_iterator type = typeReg.begin(); type != end; ++type)
	 cout << type->name() << "'s instance count:\t" << type->factory().instances() << "\n";
   } catch(const Exception& e) {
      cerr << "Moove exception encountered: " << e.what() << endl;
      return 1;
   } catch(const std::exception& e) {
      cerr << "Exception encountered: " << e.what() << endl;
      return 1;
   } catch(...) {
      cerr << "Unknown exception encountered" << endl;
      return 1;
   }

   return 0;
}


