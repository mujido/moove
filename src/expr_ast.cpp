//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "expr_ast.hpp"

#include "ast_visitor.hpp"

namespace Moove {

namespace Expr {

void Integer::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Objnum::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Str::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Real::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void List::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Variable::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Scatter::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Scatter::Target::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Not::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Negate::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Splice::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void PreInc::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void PreDec::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void PostInc::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void PostDec::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Or::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void And::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Equal::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void NotEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Less::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void LessEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void GreaterEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Greater::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void In::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Add::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Sub::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Mul::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Div::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Mod::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Exp::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Assign::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void AddEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void SubEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}
void MulEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}
void DivEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}
void ModEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void ExpEqual::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Conditional::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Index::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Range::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Catch::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Prop::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void SystemProp::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void SystemCall::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void VerbCall::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Builtin::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Length::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

}    //namespace Expr

}    //namespace Moove
