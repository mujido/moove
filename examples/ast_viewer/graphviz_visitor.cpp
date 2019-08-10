//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "graphviz_visitor.hpp"

#include <cassert>

unsigned GraphvizVisitor::Node::s_nodeCount = 0;

const char* GraphvizVisitor::PROGRAM_SHAPE = "house";
const char* GraphvizVisitor::EXPR_SHAPE = "diamond";
const char* GraphvizVisitor::STMT_SHAPE = "hexagon";
const char* GraphvizVisitor::INFO_SHAPE = "box";
const char* GraphvizVisitor::VALUE_SHAPE = "ellipse";

std::string GraphvizVisitor::escapeStr(const std::string& str)
{
   std::string result;

   std::string::size_type start = 0;
   std::string::size_type size = str.size();
   for(std::string::size_type i = 0; i != size; ++i) {
      if(str[i] == '"' || str[i] == '\\') {
	 result += str.substr(start, i - start) + '\\' + str[i];
	 start = i + 1;
      }
   }
      
   if(start < size)
      result += str.substr(start);
   
   return result;
}

void GraphvizVisitor::outputNodeDecl(const Node& node,
				     const char* shape)
{
   m_stream << '\t' << node.id()
	    << " [label=\"" << node.label() << "\",shape="
	    << shape << "]\n";
}

void GraphvizVisitor::outputEdgeTo(const std::string& label,
				   const char* shape)
{
   Node node(label);

   outputNodeDecl(node, shape);
   m_stream << '\t' << m_parentNodes.back().id() << " -> " << node.id()
	    << '\n';
}

void GraphvizVisitor::outputEdgeToType(const std::string& type,
				       const std::string& value,
				       const char* typeShape)
{
   Node typeNode(type);
   Node valueNode(value);

   outputNodeDecl(typeNode, typeShape);
   outputNodeDecl(valueNode, VALUE_SHAPE);

   m_stream << '\t' << m_parentNodes.back().id() 
	    << " -> " << typeNode.id()
	    << " -> " << valueNode.id() << '\n';
}

void GraphvizVisitor::beginParent(const std::string& label,
				  const char* shape)
{
   Node parentNode(label);
   outputNodeDecl(parentNode, shape);
   m_stream << '\t' << m_parentNodes.back().id() << " -> " 
	    << parentNode.id() << '\n';
   m_parentNodes.push_back(parentNode);
}

void GraphvizVisitor::endParent()
{
   m_parentNodes.pop_back();
}

void GraphvizVisitor::outputUnaryOp(const std::string& op,
				    const Expr::UnaryExpr& expr)
{
   beginParent(op, EXPR_SHAPE);
   expr.operand().accept(*this);
   endParent();
}

void GraphvizVisitor::outputBinaryOp(const std::string& op,
				     const Expr::BinaryExpr& expr)
{
   beginParent(op, EXPR_SHAPE);
   expr.left().accept(*this);
   expr.right().accept(*this);
   endParent();
}

void GraphvizVisitor::acceptBody(const Stmt::Block& block)
{
   beginParent("stmts");
   acceptAll(block);
   endParent();
}

void GraphvizVisitor::outputID(Symbol id)
{
   if(id)
      outputEdgeToType("id", id.name());
}

void GraphvizVisitor::visit(const Program& program)
{
   Node::resetCount();

   m_stream << "digraph ast {\n";

   Node programNode("program");

   outputNodeDecl(programNode, PROGRAM_SHAPE);
   m_parentNodes.push_back(programNode);
   acceptBody(program.stmts());
   m_parentNodes.pop_back();

   m_stream << "}\n";

   assert(m_parentNodes.empty());
}

void GraphvizVisitor::visit(const Expr::Integer& intExpr)
{
   outputEdgeToType("int", toString(intExpr.value()), EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Expr::Objnum& objnumExpr)
{
   outputEdgeToType("objnum", "#" + toString(objnumExpr.objnum()), EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Expr::Str& strExpr)
{
   outputEdgeToType("str", escapeStr(strExpr.str()), EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Expr::Real& realExpr)
{
   outputEdgeToType("real", toString(realExpr.value()), EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Expr::List& listExpr)
{
   beginParent("list", EXPR_SHAPE);
   acceptAll(listExpr.elements());
   endParent();
}

void GraphvizVisitor::visit(const Expr::Variable& varExpr)
{
   outputEdgeToType("var", varExpr.id().name(), EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Expr::Scatter& scatterExpr)
{
   beginParent("scatter", EXPR_SHAPE);
   acceptAll(scatterExpr.targets());
   endParent();
}

void GraphvizVisitor::visit(const Expr::Scatter::Target& target)
{
   typedef Expr::Scatter::Target Target;

   switch(target.type()) {
      case Target::REQUIRED:
	 outputEdgeToType("REQ", target.id().name());
	 break;
	 
      case Target::OPTIONAL:
	 if(target.hasDefault()) {
	    beginParent("OPT_VAL");
	    outputEdgeTo(target.id().name());
	    target.expr().accept(*this);
	 } else
	    outputEdgeToType("OPT", target.id().name());
	 
	 break;
	 
      case Target::REST:
	 outputEdgeToType("REST", target.id().name());
	 break;
   }
}

void GraphvizVisitor::visit(const Expr::Conditional& condExpr)
{
   beginParent("COND", EXPR_SHAPE);
   condExpr.test().accept(*this);
   condExpr.trueExpr().accept(*this);
   condExpr.falseExpr().accept(*this);
   endParent();
}

void GraphvizVisitor::visit(const Expr::Index& indexExpr)
{
   beginParent("INDEX", EXPR_SHAPE);
   indexExpr.expr().accept(*this);
   indexExpr.index().accept(*this);
   endParent();
}

void GraphvizVisitor::visit(const Expr::Range& rangeExpr)
{
   beginParent("RANGE", EXPR_SHAPE);
   rangeExpr.expr().accept(*this);
   rangeExpr.start().accept(*this);
   rangeExpr.end().accept(*this);
   endParent();
}

void GraphvizVisitor::visit(const Expr::Catch& catchExpr)
{
   beginParent("CATCH", EXPR_SHAPE);
   catchExpr.expr().accept(*this);

   beginParent("codes");

   if(catchExpr.codes().empty())
      outputEdgeTo("ALL");
   else
      acceptAll(catchExpr.codes());

   endParent();

   endParent();
}

void GraphvizVisitor::visit(const Expr::Prop& propExpr)
{
   beginParent("PROP", EXPR_SHAPE);
   propExpr.object().accept(*this);
   propExpr.name().accept(*this);
   endParent();
}

void GraphvizVisitor::visit(const Expr::VerbCall& verbExpr)
{
   beginParent("VERB_CALL", EXPR_SHAPE);
   verbExpr.object().accept(*this);
   verbExpr.name().accept(*this);
   
   beginParent("args");
   acceptAll(verbExpr.args());
   endParent();

   endParent();
}

void GraphvizVisitor::visit(const Expr::SystemProp& sysPropExpr)
{
   beginParent("SYS_PROP", EXPR_SHAPE);
   outputEdgeTo(sysPropExpr.name());
   endParent();
}

void GraphvizVisitor::visit(const Expr::SystemCall& sysCallExpr)
{
   beginParent("SYS_CALL", EXPR_SHAPE);
   outputEdgeTo(sysCallExpr.name());
   
   beginParent("args");
   acceptAll(sysCallExpr.args());
   endParent();

   endParent();
}

void GraphvizVisitor::visit(const Expr::Builtin& builtinExpr)
{
   beginParent("BUILTIN_FUNC", EXPR_SHAPE);
   outputEdgeTo(builtinExpr.name());
   
   beginParent("args");
   acceptAll(builtinExpr.args());
   endParent();

   endParent();
}

void GraphvizVisitor::visit(const Expr::Length& lengthExpr)
{
   outputEdgeTo("DOLLAR", EXPR_SHAPE);
}

void GraphvizVisitor::visit(const Stmt::If& ifStmt)
{
   beginParent("IF", STMT_SHAPE);
   ifStmt.test().accept(*this);
   acceptBody(ifStmt.body());
   acceptAll(ifStmt.elseList());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::If::ElseIf& elseStmt)
{
   if(elseStmt.hasTest()) {
      beginParent("ELSEIF", STMT_SHAPE);
      elseStmt.test().accept(*this);
   } else
      beginParent("ELSE", STMT_SHAPE);

   acceptBody(elseStmt.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::While& whileStmt)
{
   beginParent("WHILE", STMT_SHAPE);
   outputID(whileStmt.id());
   whileStmt.test().accept(*this);
   acceptBody(whileStmt.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::ForList& forStmt)
{
   beginParent("FOR_LIST", STMT_SHAPE);
   outputID(forStmt.id());
   forStmt.expr().accept(*this);
   acceptBody(forStmt.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::ForRange& forStmt)
{
   beginParent("FOR_RANGE", STMT_SHAPE);
   outputID(forStmt.id());
   forStmt.start().accept(*this);
   forStmt.end().accept(*this);
   acceptBody(forStmt.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::TryExcept& tryStmt)
{
   beginParent("TRY_EXCEPT", STMT_SHAPE);
   acceptBody(tryStmt.body());
   acceptAll(tryStmt.excepts());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::TryExcept::Except& except)
{
   beginParent("EXCEPT", STMT_SHAPE);
   outputID(except.id());
   
   beginParent("codes");
   
   if(except.codes().empty())
      outputEdgeTo("ALL", EXPR_SHAPE);
   else
      acceptAll(except.codes());

   endParent();

   acceptBody(except.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::TryFinally& tryStmt)
{
   beginParent("TRY_FINALLY", STMT_SHAPE);
   acceptBody(tryStmt.body());
   acceptBody(tryStmt.finally());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Fork& forkStmt)
{
   beginParent("FORK", STMT_SHAPE);
   outputID(forkStmt.id());
   forkStmt.delay().accept(*this);
   acceptBody(forkStmt.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Switch& switchStmt)
{
   beginParent("SWITCH", STMT_SHAPE);
   switchStmt.expr().accept(*this);
   acceptAll(switchStmt.cases());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Switch::Case& caseClause)
{
   if(!caseClause.isDefault()) {
      beginParent("CASE", STMT_SHAPE);
      caseClause.expr().accept(*this);
   } else
      beginParent("DEFAULT", STMT_SHAPE);

   acceptBody(caseClause.body());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Return& returnStmt)
{
   beginParent("RETURN", STMT_SHAPE);
   
   if(returnStmt.hasExpr())
      returnStmt.expr().accept(*this);

   endParent();
}

void GraphvizVisitor::visit(const Stmt::Continue& continueStmt)
{
   beginParent("CONTINUE", STMT_SHAPE);
   outputID(continueStmt.id());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Break& breakStmt)
{
   beginParent("BREAK", STMT_SHAPE);
   outputID(breakStmt.id());
   endParent();
}

void GraphvizVisitor::visit(const Stmt::Expr& exprStmt)
{
   beginParent("EXPR_STMT", STMT_SHAPE);
   exprStmt.expr().accept(*this);
   endParent();
}
   
