#include "script_visitor.hpp"

#include <program_ast.hpp>
#include <intvar.hpp>
#include <realvar.hpp>
#include <strvar.hpp>
#include <listvar.hpp>
#include <reply.hpp>
#include <op_package.hpp>
#include <compiler.hpp>

#include <iostream>
#include <boost/iterator/indirect_iterator.hpp>

using namespace Moove;

template<class T, class VariantType>
struct FixedCast {
   T operator () (const Variant& var)const
   { return dynamic_cast<const VariantType&>(var).value(); }
};

template<class T, class VariantType1, class VariantType2>
struct BinaryCast {
   T operator () (const Variant& var)const
   {
      if(const VariantType1* v1 = dynamic_cast<const VariantType1*>(&var))
	 return v1->value();
      else if(const VariantType2* v2 = dynamic_cast<const VariantType2*>(&var))
	 return v2->value();

      throw std::bad_cast();
      return T();
   }
};
   
template<class ResultVariant, class Cast>
class ScriptOperatorPackage : public OperatorPackage {
private:
   Cast m_cast;

   Reply doBinaryMathOp(Opcode op, const Variant& left, const Variant& right)const
   {
      boost::shared_ptr<ResultVariant> resultVar(dynamic_cast<ResultVariant*>(ResultVariant::classFactory().create()));
      assert(resultVar.get() != 0);

      switch(op) {
	 case OP_ADD:
	    resultVar->setValue(m_cast(left) + m_cast(right));
	    break;

	 case OP_SUB:
	    resultVar->setValue(m_cast(left) - m_cast(right));
	    break;

	 case OP_MUL:
	    resultVar->setValue(m_cast(left) * m_cast(right));
	    break;

	 case OP_DIV:
	    resultVar->setValue(m_cast(left) / m_cast(right));
	    break;

	 default:
	    return OperatorPackage::doBinaryOp(op, left, right);
      }

      return Reply(Reply::NORMAL, resultVar);
   }

   Reply doBinaryCompareOp(Opcode op, const Variant& left, const Variant& right)const
   {
      boost::shared_ptr<DefaultIntVar> resultVar(DefaultIntVar::classFactory().create());
      assert(resultVar.get() != 0);
      
      switch(op) {
	 case OP_LT:
	    resultVar->setValue(m_cast(left) < m_cast(right));
	    break;

	 case OP_LE:
	    resultVar->setValue(m_cast(left) <= m_cast(right));
	    break;

	 case OP_EQ:
	    resultVar->setValue(m_cast(left) == m_cast(right));
	    break;

	 case OP_NE:
	    resultVar->setValue(m_cast(left) != m_cast(right));
	    break;

	 case OP_GE:
	    resultVar->setValue(m_cast(left) >= m_cast(right));
	    break;

	 case OP_GT:
	    resultVar->setValue(m_cast(left) > m_cast(right));
	    break;

	 default:
	    return OperatorPackage::doBinaryOp(op, left, right);
      }

      return Reply(Reply::NORMAL, resultVar);
   }

public:
   Reply doBinaryOp(Opcode op, const Variant& left, const Variant& right)const
   {
      switch(op) {
	 case OP_ADD:
	 case OP_SUB:
	 case OP_MUL:
	 case OP_DIV:
	    return doBinaryMathOp(op, left, right);

	 case OP_LT:
	 case OP_LE:
	 case OP_EQ:
	 case OP_NE:
	 case OP_GE:
	 case OP_GT:
	    return doBinaryCompareOp(op, left, right);
      }

      return OperatorPackage::doBinaryOp(op, left, right);
   }
};

template<>
Reply 
ScriptOperatorPackage<DefaultStrVar, 
		      FixedCast<const std::string&, StrVar> >::doBinaryMathOp(Opcode op, const Variant& left, const Variant& right)const
{
   if(op == OP_ADD) {
      boost::shared_ptr<DefaultStrVar> resultVar(DefaultStrVar::classFactory().create());
      assert(resultVar.get() != 0);

      resultVar->setValue(m_cast(left) + m_cast(right));
      return Reply(Reply::NORMAL, resultVar);
   }

   return OperatorPackage::doBinaryOp(op, left, right);
}
      
typedef ScriptOperatorPackage<DefaultIntVar, FixedCast<int, IntVar> > IntOpPkg;
typedef ScriptOperatorPackage<DefaultRealVar, FixedCast<double, RealVar> > RealOpPkg;
typedef ScriptOperatorPackage<DefaultRealVar, BinaryCast<double, IntVar, RealVar> > IntRealOpPkg;
typedef ScriptOperatorPackage<DefaultStrVar, FixedCast<const std::string&, StrVar> > StrOpPkg;

namespace {

IntOpPkg	pkgInt;
RealOpPkg	pkgReal;
IntRealOpPkg	pkgIntReal;
StrOpPkg	pkgStr;

}

void ScriptVisitor::visitStmts(const Stmt::Block& block)
{
   Stmt::Block::const_iterator end = block.end();
   for(Stmt::Block::const_iterator stmt = block.begin(); stmt != end; ++stmt)
      (*stmt)->accept(*this);
}

void ScriptVisitor::doBinaryOp(Opcode op, const Expr::BinaryExpr& binExpr)
{
   binExpr.left().accept(*this);
   binExpr.right().accept(*this);
   m_vm.doBinaryOp(op);
}

void ScriptVisitor::assignTo(const Expr::Expr& expr)
{
   if(const Expr::Variable* var = dynamic_cast<const Expr::Variable*>(&expr))
      m_vars[var->id()] = m_vm.stackTop();
   else 
      throw InvalidAssignment("Invalid Assignment");
}

void ScriptVisitor::acceptArgs(const Expr::ArgList& args)
{
   ListVar::Container contents;

   Expr::ArgList::const_iterator end = args.end();
   for(Expr::ArgList::const_iterator elem = args.begin(); elem != end; ++elem) {
      m_spliceFlag = false;
      (*elem)->accept(*this);

      if(m_spliceFlag) {
	 boost::shared_ptr<ListVar> splice = boost::dynamic_pointer_cast<ListVar>(m_vm.popOffStack());
	 assert(splice);
	 contents.insert(contents.end(), splice->contents().begin(), splice->contents().end());
      } else {
	 contents.push_back(m_vm.popOffStack());
      }
   }

   m_spliceFlag = false;
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultListVar::classFactory().createList(contents)));
}

bool ScriptVisitor::runIf(const Expr::Expr& test, const Stmt::Block& stmts)
{
   test.accept(*this);
   boost::shared_ptr<Variant> result = m_vm.popOffStack();

   bool shouldRun = false;

   if(boost::shared_ptr<IntVar> intVar = boost::dynamic_pointer_cast<IntVar>(result))
      shouldRun = intVar->value() != 0;
   else if(boost::shared_ptr<RealVar> realVar = boost::dynamic_pointer_cast<RealVar>(result))
      shouldRun = realVar->value() != 0;
   else if(boost::shared_ptr<StrVar> strVar = boost::dynamic_pointer_cast<StrVar>(result))
      shouldRun = !strVar->value().empty();

   if(shouldRun)
      visitStmts(stmts);

   return shouldRun;
}

ScriptVisitor::ScriptVisitor() : m_spliceFlag(false), m_vm(m_opMap)
{
   const TypeRegistry::TypeEntry& intType = m_typeReg.registerType("int", DefaultIntVar::classFactory());
   const TypeRegistry::TypeEntry& realType = m_typeReg.registerType("real", DefaultRealVar::classFactory());
   const TypeRegistry::TypeEntry& strType = m_typeReg.registerType("str", DefaultStrVar::classFactory());

   m_opMap.registerUnary(intType, pkgInt);
   m_opMap.registerUnary(realType, pkgReal);
   m_opMap.registerUnary(strType, pkgStr);
   
   m_opMap.registerBinary(intType, intType, pkgInt);
   m_opMap.registerBinary(realType, realType, pkgReal);
   m_opMap.registerBinary(intType, realType, pkgIntReal);
   m_opMap.registerBinary(realType, intType, pkgIntReal);
   m_opMap.registerBinary(strType, strType, pkgStr);
}

void ScriptVisitor::visit(const Program& program)
{
   m_vars.clear();
   m_vm.reset();
   m_program = &program;

   visitStmts(program.stmts());
}
      
void ScriptVisitor::visit(const Expr::Integer& intExpr)
{
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(intExpr.value())));
}

void ScriptVisitor::visit(const Expr::Real& realExpr)
{
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultRealVar::classFactory().createReal(realExpr.value())));
}

void ScriptVisitor::visit(const Expr::Str& strExpr)
{
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultStrVar::classFactory().createStr(strExpr.str())));
}

void ScriptVisitor::visit(const Expr::List& listExpr)
{
   acceptArgs(listExpr.elements());
}

void ScriptVisitor::visit(const Expr::Variable& varExpr)
{
   VariableTable::const_iterator var = m_vars.find(varExpr.id());
   if(var == m_vars.end())
      throw UndefinedVariable("Undefined Variable");
  
   m_vm.pushOnStack(var->second);
}

void ScriptVisitor::visit(const Expr::Splice& spliceExpr)
{
   spliceExpr.operand().accept(*this);
   m_spliceFlag = true;
}
   
void ScriptVisitor::visit(const Expr::PreInc& preIncExpr)
{
   preIncExpr.operand().accept(*this);
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(1)));
   m_vm.doBinaryOp(OP_ADD);
   assignTo(preIncExpr.operand());
}

void ScriptVisitor::visit(const Expr::PreDec& preDecExpr)
{
   preDecExpr.operand().accept(*this);
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(-1)));
   m_vm.doBinaryOp(OP_ADD);
   assignTo(preDecExpr.operand());
}

void ScriptVisitor::visit(const Expr::PostInc& postIncExpr)
{
   postIncExpr.operand().accept(*this);
   m_vm.pushOnStack(m_vm.stackTop());
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(1)));
   m_vm.doBinaryOp(OP_ADD);
   assignTo(postIncExpr.operand());
   m_vm.popOffStack();
}

void ScriptVisitor::visit(const Expr::PostDec& postDecExpr)
{
   postDecExpr.operand().accept(*this);
   m_vm.pushOnStack(m_vm.stackTop());
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(-1)));
   m_vm.doBinaryOp(OP_ADD);
   assignTo(postDecExpr.operand());
   m_vm.popOffStack();
}

void ScriptVisitor::visit(const Expr::Assign& assignExpr)
{
   assignExpr.right().accept(*this);
   assignTo(assignExpr.left());
}

void ScriptVisitor::visit(const Expr::AddEqual& addEqExpr)
{
   doBinaryOp(OP_ADD, addEqExpr);
   assignTo(addEqExpr.left());
}

void ScriptVisitor::visit(const Expr::SubEqual& subEqExpr)
{
   doBinaryOp(OP_SUB, subEqExpr);
   assignTo(subEqExpr.left());
}

void ScriptVisitor::visit(const Expr::MulEqual& mulEqExpr)
{
   doBinaryOp(OP_MUL, mulEqExpr);
   assignTo(mulEqExpr.left());
}

void ScriptVisitor::visit(const Expr::DivEqual& divEqExpr)
{
   doBinaryOp(OP_DIV, divEqExpr);
   assignTo(divEqExpr.left());
}

void ScriptVisitor::visit(const Expr::Conditional& condExpr)
{
   condExpr.test().accept(*this);
   boost::shared_ptr<IntVar> result = boost::dynamic_pointer_cast<IntVar>(m_vm.popOffStack());
   assert(result);

   if(result->value())
      condExpr.trueExpr().accept(*this);
   else
      condExpr.falseExpr().accept(*this);
}

void ScriptVisitor::visit(const Expr::Index& expr)
{
   expr.expr().accept(*this);
   expr.index().accept(*this);

   boost::shared_ptr<IntVar> index = boost::dynamic_pointer_cast<IntVar>(m_vm.popOffStack());
   boost::shared_ptr<ListVar> value = boost::dynamic_pointer_cast<ListVar>(m_vm.popOffStack());
   assert(index && value);

   if(index->value() >= 1 && index->value() <= value->contents().size())
      m_vm.pushOnStack(value->contents()[index->value() - 1]);
   else
      throw OutOfRange("Index value out of range");
}

void ScriptVisitor::printVariant(const Variant& var)
{
   if(const IntVar* intValue = dynamic_cast<const IntVar*>(&var)) {
      std::cout << intValue->value();
   } else if(const RealVar* realValue = dynamic_cast<const RealVar*>(&var)) {
      std::cout << realValue->value();
   } else if(const StrVar* strValue = dynamic_cast<const StrVar*>(&var)) {
      std::cout << strValue->value();
   } else if(const ListVar* listValue = dynamic_cast<const ListVar*>(&var)) {
      std::cout << "{";
      
      ListVar::Container::const_iterator end = listValue->contents().end();
      for(ListVar::Container::const_iterator elem = listValue->contents().begin(); elem != end; ++elem) {
	 printVariant(**elem);
	 
	 if(elem + 1 != end)
	    std::cout << ", ";
      }

      std::cout << "}";
   } else {
      std::cout << "(unknown type: " << var.factory().regEntry().name() << ")";
   }
}

void ScriptVisitor::visit(const Expr::Builtin& builtinExpr)
{
   typedef boost::indirect_iterator<ListVar::Container::const_iterator> ArgIterator;

   boost::shared_ptr<Variant> resultVar;
   boost::shared_ptr<ListVar> argsVar;

   acceptArgs(builtinExpr.args());
   if(!(argsVar = boost::dynamic_pointer_cast<ListVar>(m_vm.popOffStack())))
      throw InvalidType("Invalid Type");

   if(builtinExpr.name() == "print") {
      std::for_each(ArgIterator(argsVar->contents().begin()), ArgIterator(argsVar->contents().end()), &printVariant);
      std::cout << std::endl;
   } else if(builtinExpr.name() == "input") {
      assert(argsVar->contents().size() == 1);

      boost::shared_ptr<StrVar> inputType = boost::dynamic_pointer_cast<StrVar>(argsVar->contents()[0]);
      assert(inputType);

      std::cout << "Enter value: ";

      if(inputType->value() == "int") {
	 int value;
	 
	 assert(std::cin >> value);
	 std::cin.ignore();
	 resultVar.reset(DefaultIntVar::classFactory().createInt(value));
      } else if(inputType->value() == "real") {
	 double value;

	 assert(std::cin >> value);
	 std::cin.ignore();
	 resultVar.reset(DefaultRealVar::classFactory().createReal(value));
      } else if(inputType->value() == "str") {
	 std::string value;

	 assert(getline(std::cin, value));
	 resultVar.reset(DefaultStrVar::classFactory().createStr(value));
      } else {
	 throw InvalidType("Invalid Type");
      }
   }

   // this is an expression, so it needs a return value
   m_vm.pushOnStack(resultVar ? resultVar : boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(0)));
}

void ScriptVisitor::visit(const Expr::Length&)
{
   boost::shared_ptr<ListVar> listValue = boost::dynamic_pointer_cast<ListVar>(m_vm.stackTop());
   m_vm.pushOnStack(boost::shared_ptr<Variant>(DefaultIntVar::classFactory().createInt(listValue->contents().size())));
}

void ScriptVisitor::visit(const Stmt::If& ifStmt)
{
   if(!runIf(ifStmt.test(), ifStmt.body())) {
      Stmt::If::ElseList::const_iterator clause, end = ifStmt.elseList().end();
      for(clause = ifStmt.elseList().begin(); clause != end; ++clause) {
	 m_ranIf = false;
	 (*clause)->accept(*this);

	 if(m_ranIf)
	    break;
      }
   }
}

void ScriptVisitor::visit(const Stmt::If::Else& elseClause)
{
   if(elseClause.hasTest())		// elseif
      m_ranIf = runIf(elseClause.test(), elseClause.body());
   else {				// else
      visitStmts(elseClause.body());
      m_ranIf = true;
   }
}
   
void ScriptVisitor::visit(const Stmt::While& whileStmt)
{
   for(;;) {
      whileStmt.test().accept(*this);
      boost::shared_ptr<IntVar> testResult = boost::dynamic_pointer_cast<IntVar>(m_vm.popOffStack());
      assert(testResult.get() != 0);

      if(!testResult->value())
	 break;

      visitStmts(whileStmt.body());
   }
}
		 
void ScriptVisitor::visit(const Stmt::ForRange& forRangeStmt)
{
   forRangeStmt.start().accept(*this);
   boost::shared_ptr<Variant> indexVar = m_vm.stackTop();

   forRangeStmt.end().accept(*this);
   boost::shared_ptr<Variant> endVar = m_vm.stackTop();

   for(;;) {
      m_vm.doBinaryOp(OP_LE);
      boost::shared_ptr<IntVar> compResult = boost::dynamic_pointer_cast<IntVar>(m_vm.popOffStack());
      assert(compResult);
      
      if(compResult->value()) {
	 m_vars[forRangeStmt.id()] = indexVar;
	 visitStmts(forRangeStmt.body());
	 
	 m_vm.pushOnStack(indexVar);
	 m_vm.pushOnStack(boost::shared_ptr<DefaultIntVar>(DefaultIntVar::classFactory().createInt(1)));
	 m_vm.doBinaryOp(OP_ADD);
	 indexVar = m_vm.stackTop();
	 m_vm.pushOnStack(endVar);
      } else {
	 break;
      }
   }
}

void ScriptVisitor::visit(const Stmt::Expr& exprStmt)
{
   exprStmt.expr().accept(*this);
   m_vm.popOffStack();
}

