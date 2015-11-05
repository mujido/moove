//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_PARTIAL_AST_VISITOR_HPP
#define MOOVE_PARTIAL_AST_VISITOR_HPP

#include "ast_visitor.hpp"

namespace Moove {

class PartialASTVisitor : public ASTVisitor {
public:
   typedef std::logic_error UnimplementedVisitor;

protected:
   //Method called by default from Expr::Expr handlers
   virtual void unhandledExpr(const Expr::Expr& expr)
   { throw UnimplementedVisitor("Moove: Unimplemented AST visitor"); }

   //Method called by default from Stmt::Stmt handlers
   virtual void unhandledStmt(const Stmt::Stmt& stmt)
   { throw UnimplementedVisitor("Moove: Unimplemented AST visitor"); }

public:
   //visit(const Program&) unimplemented

   //Expression AST
   virtual void visit(const Expr::Integer& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Objnum& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Str& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Real& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::List& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Variable& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Scatter& expr)
   { unhandledExpr(expr); }

   //visit(const Expr::Scatter::Target&) is not implemented

   virtual void visit(const Expr::Not& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Negate& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Splice& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::PreInc& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::PreDec& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::PostInc& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::PostDec& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Or& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::And& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Equal& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::NotEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Less& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::LessEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::GreaterEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Greater& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::In& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Add& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Sub& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Mul& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Div& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Mod& expr)
   { unhandledExpr(expr); }
   
   virtual void visit(const Expr::Exp& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Assign& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::AddEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::SubEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::MulEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::DivEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::ModEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::ExpEqual& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Conditional& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Index& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Range& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Catch& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Prop& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::SystemProp& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::SystemCall& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::VerbCall& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Builtin& expr)
   { unhandledExpr(expr); }

   virtual void visit(const Expr::Length& expr)
   { unhandledExpr(expr); }

   //Statement AST
   virtual void visit(const Stmt::If& stmt)
   { unhandledStmt(stmt); }

   //visit(const Stmt::If::Else&) is not implemented

   virtual void visit(const Stmt::While& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::ForList& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::ForRange& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::TryExcept& stmt)
   { unhandledStmt(stmt); }

   //visit(const Stmt::TryExcept::Except&) is not implemented

   virtual void visit(const Stmt::TryFinally& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::Fork& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::Switch& stmt)
   { unhandledStmt(stmt); }

   //visit(const Stmt::Switch::Case&) is not implemented

   virtual void visit(const Stmt::Return& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::Continue& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::Break& stmt)
   { unhandledStmt(stmt); }

   virtual void visit(const Stmt::Expr& stmt)
   { unhandledStmt(stmt); }
};

}   //namespace Moove

#endif   //MOOVE_PARTIAL_AST_VISITOR_HPP
