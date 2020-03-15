//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "stmt_ast.hpp"

#include "ast_visitor.hpp"

namespace Moove {

namespace Stmt {

void If::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void If::Else::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void While::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void ForList::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void ForRange::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void TryExcept::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void TryExcept::Except::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void TryFinally::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Fork::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Switch::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Switch::Case::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Return::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Continue::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void Break::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

void ExprStmt::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

}    //namespace Stmt

}    //namespace Moove

