//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#ifndef GRAPHVIZ_VISITOR_HPP
#define GRAPHVIZ_VISITOR_HPP

#include "ast_visitor.hpp"
#include "memory.hpp"
#include "parser.hpp"

#include <iosfwd>
#include <list>
#include <string>
#include <sstream>

using namespace Moove;

class GraphvizVisitor : public ASTVisitor {
private:
   class Node {
   private:
      std::string m_label;
      unsigned    m_id;

      static unsigned s_nodeCount;

   public:
      Node(const std::string& label) : m_label(label), m_id(++s_nodeCount)
      {}

      //Retrieve the label of the node as seen in the graph
      const std::string& label()const
      { return m_label; }

      //Retrieve the unique id of this node
      unsigned id()const
      { return m_id; }

      //Resets node count to 0
      static void resetCount()
      { s_nodeCount = 0; }
   };

   typedef std::list<Node> NodeStack;

   NodeStack     m_parentNodes;
   std::ostream& m_stream;

   static const char* PROGRAM_SHAPE; //node shape for program
   static const char* EXPR_SHAPE;    //node shape for expressions
   static const char* STMT_SHAPE;    //node shape for statements
   static const char* INFO_SHAPE;    //node shape for info
   static const char* VALUE_SHAPE;   //node shape for data values

   template<class Cont>
   void acceptAll(const Cont& cont)
   {
      typename Cont::const_iterator end = cont.end();
      for(typename Cont::const_iterator i = cont.begin(); i != end; ++i)
	 (*i)->accept(*this);
   }

   //Output a Node declaration, which binds its unique name to its label
   void outputNodeDecl(const Node& node, const char* shape);

   //Output an edge between last element in m_parentNodes and node
   void outputEdgeTo(const std::string& label, const char* shape = INFO_SHAPE);

   //Output an edge between last element in m_parentNodes and a special set
   //of 2 nodes, where the first node, type, is the name of the type that
   //is given by a child node with value
   void outputEdgeToType(const std::string& type, 
			 const std::string& value,
			 const char* typeShape = INFO_SHAPE);

   //Output an edge between last element in m_parentNodes and place the node
   //at the end of m_parentNodes.
   void beginParent(const std::string& label, const char* shape = INFO_SHAPE);

   //Removes the last node from the m_parentNodes stack
   void endParent();

   //Output a unary operator and operand
   void outputUnaryOp(const std::string& op, const Expr::UnaryExpr& expr);

   //Output a binary operator and operands
   void outputBinaryOp(const std::string& op, const Expr::BinaryExpr& expr);

   //Output a statement block
   void acceptBody(const Stmt::Block& block);

   //Output a loop/etc id
   void outputID(Symbol id);

   //Convert a value to a string
   template<class T>
   static std::string toString(T value)
   {
      std::ostringstream str;
      str << value;
      return str.str();
   }

   //Escape a string
   static std::string escapeStr(const std::string& str);

public:
   GraphvizVisitor(std::ostream& stream) : m_stream(stream)
   {}

   void visit(const Program& program);

   //Expression AST
   void visit(const Expr::Integer&);

   void visit(const Expr::Objnum&);

   void visit(const Expr::Str&);

   void visit(const Expr::Real&);

   void visit(const Expr::List&);

   void visit(const Expr::Variable&);

   void visit(const Expr::Scatter&);

   void visit(const Expr::Scatter::Target&);

   void visit(const Expr::Not& notExpr)
   { outputUnaryOp("NOT", notExpr); }

   void visit(const Expr::Negate& negateExpr)
   { outputUnaryOp("NEG", negateExpr); }

   void visit(const Expr::Splice& spliceExpr)
   { outputUnaryOp("SPLICE", spliceExpr); }

   void visit(const Expr::PreInc& preIncExpr)
   { outputUnaryOp("PRE_INC", preIncExpr); }

   void visit(const Expr::PreDec& preDecExpr)
   { outputUnaryOp("PRE_DEC", preDecExpr); }

   void visit(const Expr::PostInc& postIncExpr)
   { outputUnaryOp("POST_INC", postIncExpr); }

   void visit(const Expr::PostDec& postDecExpr)
   { outputUnaryOp("POST_DEC", postDecExpr); }

   void visit(const Expr::Or& orExpr)
   { outputBinaryOp("OR", orExpr); }

   void visit(const Expr::And& andExpr)
   { outputBinaryOp("AND", andExpr); }

   void visit(const Expr::Equal& equalExpr)
   { outputBinaryOp("EQUAL", equalExpr); }

   void visit(const Expr::NotEqual& notEqualExpr)
   { outputBinaryOp("NOT_EQUAL", notEqualExpr); }

   void visit(const Expr::Less& lessExpr)
   { outputBinaryOp("LESS", lessExpr); }

   void visit(const Expr::LessEqual& lessEqualExpr)
   { outputBinaryOp("LESS_EQUAL", lessEqualExpr); }

   void visit(const Expr::GreaterEqual& greaterEqualExpr)
   { outputBinaryOp("GREATER_EQUAL", greaterEqualExpr); }

   void visit(const Expr::Greater& greaterExpr)
   { outputBinaryOp("GREATER", greaterExpr); }

   void visit(const Expr::In& inExpr)
   { outputBinaryOp("IN", inExpr); }

   void visit(const Expr::Add& addExpr)
   { outputBinaryOp("ADD", addExpr); }

   void visit(const Expr::Sub& subExpr)
   { outputBinaryOp("SUB", subExpr); }

   void visit(const Expr::Mul& mulExpr)
   { outputBinaryOp("MUL", mulExpr); }

   void visit(const Expr::Div& divExpr)
   { outputBinaryOp("DIV", divExpr); }

   void visit(const Expr::Mod& modExpr)
   { outputBinaryOp("MOD", modExpr); }
   
   void visit(const Expr::Exp& expExpr)
   { outputBinaryOp("EXP", expExpr); }

   void visit(const Expr::Assign& assignExpr)
   { outputBinaryOp("ASSIGN", assignExpr); }

   void visit(const Expr::AddEqual& addEqualExpr)
   { outputBinaryOp("ADD_EQUAL", addEqualExpr); }

   void visit(const Expr::SubEqual& subEqualExpr)
   { outputBinaryOp("SUB_EQUAL", subEqualExpr); }

   void visit(const Expr::MulEqual& mulEqualExpr)
   { outputBinaryOp("MUL_EQUAL", mulEqualExpr); }

   void visit(const Expr::DivEqual& divEqualExpr)
   { outputBinaryOp("DIV_EQUAL", divEqualExpr); }

   void visit(const Expr::ModEqual& modEqualExpr)
   { outputBinaryOp("MOD_EQUAL", modEqualExpr); }

   void visit(const Expr::ExpEqual& expEqualExpr)
   { outputBinaryOp("EXP_EQUAL", expEqualExpr); }

   void visit(const Expr::Conditional&);

   void visit(const Expr::Index&);

   void visit(const Expr::Range&);

   void visit(const Expr::Catch&);

   void visit(const Expr::Prop&);

   void visit(const Expr::SystemProp&);

   void visit(const Expr::SystemCall&);

   void visit(const Expr::VerbCall&);

   void visit(const Expr::Builtin&);

   void visit(const Expr::Length&);

   //Statement AST
   void visit(const Stmt::If&);

   void visit(const Stmt::If::Else&);

   void visit(const Stmt::While&);

   void visit(const Stmt::ForList&);

   void visit(const Stmt::ForRange&);

   void visit(const Stmt::TryExcept&);

   void visit(const Stmt::TryExcept::Except&);

   void visit(const Stmt::TryFinally&);

   void visit(const Stmt::Fork&);

   void visit(const Stmt::Switch&);

   void visit(const Stmt::Switch::Case&);

   void visit(const Stmt::Return&);

   void visit(const Stmt::Continue&);

   void visit(const Stmt::Break&);

   void visit(const Stmt::Expr&);
};

#endif
