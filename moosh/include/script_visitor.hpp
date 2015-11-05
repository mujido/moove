/**
 * \file
 */
#ifndef MOOSH_SCRIPT_VISITOR_HPP
#define MOOSH_SCRIPT_VISITOR_HPP

#include <ast_visitor.hpp>
#include <symbol_table.hpp>
#include <type_registry.hpp>
#include <op_map.hpp>
#include <variant.hpp>
#include <opcodes.hpp>
#include <vm.hpp>

#include <map>
#include <stdexcept>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <memory>

class ScriptVisitor : public Moove::ASTVisitor {
private:
   typedef std::map<Moove::Symbol, boost::shared_ptr<Moove::Variant> > VariableTable;

   VariableTable		m_vars;
   Moove::TypeRegistry		m_typeReg;
   Moove::OperatorMap		m_opMap;
   const Moove::Program*	m_program;
   bool				m_ranIf;
   bool				m_spliceFlag;
   Moove::VirtualMachine        m_vm;

   void visitStmts(const Moove::Stmt::Block& block);
   
   void doBinaryOp(Moove::Opcode op, const Moove::Expr::BinaryExpr& binExpr);

   void assignTo(const Moove::Expr::Expr& expr);

   template<class Cont>
   void visitAll(const Cont& cont)
   {
      typename Cont::const_iterator end = cont.end();
      for(typename Cont::const_iterator elem = cont.begin(); elem != end; ++elem)
	 (*elem)->accept(*this);
   }

   void acceptArgs(const Moove::Expr::ArgList& args);

   bool runIf(const Moove::Expr::Expr& test, const Moove::Stmt::Block& stmts);

   static void printVariant(const Moove::Variant& var);

public:
   typedef std::runtime_error UndefinedVariable;
   typedef std::runtime_error InvalidAssignment;
   typedef std::logic_error   InvalidType;
   typedef std::runtime_error OutOfRange;

   ScriptVisitor();

   void visit(const Moove::Program& program);

   //Expression AST
   void visit(const Moove::Expr::Integer&);

   void visit(const Moove::Expr::Objnum&)
   {}

   void visit(const Moove::Expr::Str&);

   void visit(const Moove::Expr::Real&);

   void visit(const Moove::Expr::List&);

   void visit(const Moove::Expr::Variable&);

   void visit(const Moove::Expr::Scatter&)
   {}

   void visit(const Moove::Expr::Scatter::Target&)
   {}

   void visit(const Moove::Expr::Not&)
   {}

   void visit(const Moove::Expr::Negate&)
   {}

   void visit(const Moove::Expr::Splice&);

   void visit(const Moove::Expr::PreInc&);

   void visit(const Moove::Expr::PreDec&);

   void visit(const Moove::Expr::PostInc&);

   void visit(const Moove::Expr::PostDec&);

   void visit(const Moove::Expr::Or& orExpr)
   { doBinaryOp(Moove::OP_OR, orExpr); }

   void visit(const Moove::Expr::And& andExpr)
   { doBinaryOp(Moove::OP_AND, andExpr) ;}

   void visit(const Moove::Expr::Equal& eqExpr)
   { doBinaryOp(Moove::OP_EQ, eqExpr); }

   void visit(const Moove::Expr::NotEqual& neExpr)
   { doBinaryOp(Moove::OP_NE, neExpr); }

   void visit(const Moove::Expr::Less& ltExpr)
   { doBinaryOp(Moove::OP_LT, ltExpr); }

   void visit(const Moove::Expr::LessEqual& leExpr)
   { doBinaryOp(Moove::OP_LE, leExpr); }

   void visit(const Moove::Expr::GreaterEqual& geExpr)
   { doBinaryOp(Moove::OP_GE, geExpr); }

   void visit(const Moove::Expr::Greater& gtExpr)
   { doBinaryOp(Moove::OP_GT, gtExpr); }

   void visit(const Moove::Expr::In&)
   {}

   void visit(const Moove::Expr::Add& addExpr)
   { doBinaryOp(Moove::OP_ADD, addExpr); }

   void visit(const Moove::Expr::Sub& subExpr)
   { doBinaryOp(Moove::OP_SUB, subExpr); }

   void visit(const Moove::Expr::Mul& mulExpr)
   { doBinaryOp(Moove::OP_MUL, mulExpr); }

   void visit(const Moove::Expr::Div& divExpr)
   { doBinaryOp(Moove::OP_DIV, divExpr); }

   void visit(const Moove::Expr::Mod&)
   {}
   
   void visit(const Moove::Expr::Exp&)
   {}

   void visit(const Moove::Expr::Assign&);

   void visit(const Moove::Expr::AddEqual&);

   void visit(const Moove::Expr::SubEqual&);

   void visit(const Moove::Expr::MulEqual&);

   void visit(const Moove::Expr::DivEqual&);

   void visit(const Moove::Expr::ModEqual&)
   {}

   void visit(const Moove::Expr::ExpEqual&)
   {}

   void visit(const Moove::Expr::Conditional&);

   void visit(const Moove::Expr::Index&);

   void visit(const Moove::Expr::Range&)
   {}

   void visit(const Moove::Expr::Catch&)
   {}

   void visit(const Moove::Expr::Prop&)
   {}

   void visit(const Moove::Expr::SystemProp&)
   {}

   void visit(const Moove::Expr::SystemCall&)
   {}

   void visit(const Moove::Expr::VerbCall&)
   {}

   void visit(const Moove::Expr::Builtin&);

   void visit(const Moove::Expr::Length&);

   //Statement AST
   void visit(const Moove::Stmt::If&);

   void visit(const Moove::Stmt::If::Else&);

   void visit(const Moove::Stmt::While&);

   void visit(const Moove::Stmt::ForList&)
   {}

   void visit(const Moove::Stmt::ForRange&);

   void visit(const Moove::Stmt::TryExcept&)
   {}

   void visit(const Moove::Stmt::TryExcept::Except&)
   {}

   void visit(const Moove::Stmt::TryFinally&)
   {}

   void visit(const Moove::Stmt::Fork&)
   {}

   void visit(const Moove::Stmt::Switch&)
   {}

   void visit(const Moove::Stmt::Switch::Case&)
   {}

   void visit(const Moove::Stmt::Return&)
   {}

   void visit(const Moove::Stmt::Continue&)
   {}

   void visit(const Moove::Stmt::Break&)
   {}

   void visit(const Moove::Stmt::Expr&);
};

#endif   //MOOSH_SCRIPT_VISITOR_HPP
