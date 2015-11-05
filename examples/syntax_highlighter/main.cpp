//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "ast_visitor.hpp"
#include "parser.hpp"
#include "parser_msgs.hpp"
#include "highlight.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <typeinfo>

#ifdef CHECK_EXCEPT
#include <stdexcept>
#endif

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

using namespace std;
using namespace Moove;
using namespace Extra;

struct MyMessages : public ParserMessages {
   void error(const string& msg, unsigned lineNum)
   {
      cerr << "Error, line " << lineNum << ": " << msg << endl;
   }

   void warning(const string& msg, unsigned lineNum)
   {
      cerr << "Warning, line " << lineNum << ": " << msg << endl;
   }
};

class IndentManip {
private:
   unsigned m_depth;
   
public:
   static const unsigned BLOCK_INDENT = 2;
   
   IndentManip() : m_depth(0)
   {}

   unsigned indentSize()const
   { return m_depth * BLOCK_INDENT; }

   void beginBlock()
   { ++m_depth; }

   void endBlock()
   { --m_depth; }
};

template<class CharT, class Traits>
basic_ostream<CharT, Traits>& operator << (basic_ostream<CharT, Traits>& str,
					   IndentManip manip)
{
   str << setw(manip.indentSize()) << "";
   return str;
}

class TestSyntaxHighlighter : public SyntaxHighlighter<string> {
private:
   bool m_colorize;

   unsigned encodeColor(HighlightAttributes::Color color)const
   {
      switch(color) {
	 case HighlightAttributes::BLACK:
	    return 0;

	 case HighlightAttributes::RED:
	    return 1;

	 case HighlightAttributes::GREEN:
	    return 2;
	    
	 case HighlightAttributes::YELLOW:
	    return 3;

	 case HighlightAttributes::BLUE:
	    return 4;

	 case HighlightAttributes::MAGENTA:
	    return 5;

	 case HighlightAttributes::CYAN:
	    return 6;
	    
	 case HighlightAttributes::WHITE:
	    return 7;
      }

      return 0;
   }
   
   string buildStr(HighlightAttributes attrs)const
   {
      ostringstream stream;

      stream << "\x1B[0;"
             << encodeColor(attrs.fg()) + 30 << ';'
	     << encodeColor(attrs.bg()) + 40;

      if(attrs.bold())
	 stream << ";1";

      if(attrs.underline())
	 stream << ";4";

      stream << 'm';
      return stream.str();
   }

public:
   TestSyntaxHighlighter(bool colorize = true) : m_colorize(colorize)
   {
      setAttrs(IDENTIFIER, HighlightAttributes(HighlightAttributes::GREEN));
      setAttrs(RESERVED_WORD, HighlightAttributes(HighlightAttributes::WHITE,
						  true));
      setAttrs(SYMBOL, HighlightAttributes(HighlightAttributes::YELLOW));
      setAttrs(STRING, HighlightAttributes(HighlightAttributes::RED));
      setAttrs(INTEGER, HighlightAttributes(HighlightAttributes::WHITE));
      setAttrs(REAL, HighlightAttributes(HighlightAttributes::CYAN));
      setAttrs(OBJNUM, HighlightAttributes(HighlightAttributes::MAGENTA));
   }

   bool colorize()const
   { return m_colorize; }

   void enableColor(bool colorize = true)
   { m_colorize = colorize; }

   string highlight(SyntaxElement elem, const string& str)const
   {
      if(m_colorize)
	 return buildStr(attrs(elem)) + str + buildStr(defaultAttrs());
      else
	 return str;
   }
};

class TestVisitor : public ASTVisitor {
private:
   struct PrecedenceInfo {
      enum Precedence {
	 PREC_NONE,
	 PREC_ASSIGN,
	 PREC_CONDITIONAL,
	 PREC_LOGICAL,
	 PREC_COMPARISON,
	 PREC_ADDITIVE,
	 PREC_MULTIPLICATIVE,
	 PREC_EXPONENTIAL,
	 PREC_UNARY,
	 PREC_POSTFIX,
	 PREC_PRIMARY
      };

      enum ParenRule {
	 PAREN_NONE, PAREN_LESS, PAREN_LESS_OR_EQUAL
      };

      Precedence prec;
      ParenRule  rule;
   };

   ostream& m_output;

   IndentManip            m_indent;
   vector<PrecedenceInfo> m_precedStack;
   TestSyntaxHighlighter  m_hlt;

   static bool isIDSuffix(char ch)
   { return isalnum(ch) || ch == '_'; }

   void outputParen(const char* paren, 
		    const PrecedenceInfo& parentInfo,
		    PrecedenceInfo::Precedence childPrec)
   {
      if(parentInfo.rule != PrecedenceInfo::PAREN_NONE &&
	 (parentInfo.prec > childPrec || 
	  (parentInfo.prec == childPrec && 
	   parentInfo.rule == PrecedenceInfo::PAREN_LESS_OR_EQUAL)))
	 m_output << m_hlt.symbol(paren);
   }

   void beginExpr(PrecedenceInfo::Precedence prec)
   {
      outputParen("(", m_precedStack.back(), prec);
      PrecedenceInfo info = {prec, PrecedenceInfo::PAREN_LESS};
      m_precedStack.push_back(info);
   }

   void endExpr()
   {
      outputParen(")", 
		  *(m_precedStack.end() - 2), 
		  m_precedStack.back().prec);
      m_precedStack.pop_back();
   }

   void beginStmt()
   {
      PrecedenceInfo info = {PrecedenceInfo::PREC_NONE, 
			     PrecedenceInfo::PAREN_NONE};
      m_precedStack.push_back(info);
   }

   void endStmt()
   {
      m_precedStack.pop_back();
   }

   void acceptNA(const Expr::Expr& expr)
   {
      m_precedStack.back().rule = PrecedenceInfo::PAREN_NONE;
      expr.accept(*this);
   }

   template<class T>
   void acceptNA(const T& t)
   {
      t.accept(*this);
   }

   void acceptLT(const Expr::Expr& expr)
   {
      m_precedStack.back().rule = PrecedenceInfo::PAREN_LESS;
      expr.accept(*this);
   }

   void acceptLE(const Expr::Expr& expr)
   {
      m_precedStack.back().rule = PrecedenceInfo::PAREN_LESS_OR_EQUAL;
      expr.accept(*this);
   }

   string escapeStr(const string& str)
   {
      string result;

      string::size_type start = 0;
      string::size_type size = str.size();
      for(string::size_type i = 0; i != size; ++i) {
	 if(str[i] == '"' || str[i] == '\\') {
	    result += str.substr(start, i - start) + '\\' + str[i];
	    start = i + 1;
	 }
      }
      
      if(start < size)
	 result += str.substr(start);

      return result;
   }

   template<class Iter>
   void listExpansion(Iter start, Iter end)
   {
      if(start != end) {
	 acceptNA(**start);

	 while(++start != end) {
	    m_output << m_hlt.symbol(",") << ' ';
	    acceptNA(**start);
	 }
      }
   }

   void preUnaryExpr(const char* op, const Expr::UnaryExpr& expr)
   {
      beginExpr(PrecedenceInfo::PREC_UNARY);
      m_output << m_hlt.symbol(op);
      acceptLT(expr.operand());
      endExpr();
   }

   void postUnaryExpr(const char* op, const Expr::UnaryExpr& expr)
   {
      beginExpr(PrecedenceInfo::PREC_UNARY);
      acceptLT(expr.operand());
      m_output << m_hlt.symbol(op);
      endExpr();
   }

   void binaryExprLeft(const char* op, 
		       const Expr::BinaryExpr& expr,
		       PrecedenceInfo::Precedence prec,
		       TestSyntaxHighlighter::SyntaxElement elem =
		          TestSyntaxHighlighter::SYMBOL)
   {
      beginExpr(prec);
      acceptLT(expr.left());
      m_output << ' ' << m_hlt.highlight(elem, op) << ' ';
      acceptLE(expr.right());
      endExpr();
   }

   void binaryExprRight(const char* op,
			const Expr::BinaryExpr& expr,
			PrecedenceInfo::Precedence prec)
   {
      beginExpr(prec);
      acceptLE(expr.left());
      m_output << ' ' << m_hlt.symbol(op) << ' ';
      acceptLT(expr.right());
      endExpr();
   }

   void binaryExprNA(const char* op,
		     const Expr::BinaryExpr& expr,
		     PrecedenceInfo::Precedence prec)
   {
      beginExpr(prec);
      acceptNA(expr.left());
      m_output << ' ' << m_hlt.symbol(op) << ' ';
      acceptNA(expr.right());
      endExpr();
   }

   void outputPropOrVerbName(const Expr::Expr& expr)
   {
      if(const Expr::Str* id = dynamic_cast<const Expr::Str*>(&expr)) {
	 if(!id->str().empty() && 
	    (isalpha(id->str()[0]) || id->str()[0] == '_') &&
	    find_if(id->str().begin() + 1, id->str().end(), 
		    not1(ptr_fun(&isIDSuffix))) == id->str().end()) {
	    m_output << m_hlt.id(id->str());
	    return;
	 }
      }

      m_output << m_hlt.symbol("(");
      acceptNA(expr);
      m_output << m_hlt.symbol(")");
   }

   template<class Cont>
   void acceptAll(const Cont& cont)
   {
      typename Cont::const_iterator end = cont.end();
      for(typename Cont::const_iterator i = cont.begin(); i != end; ++i)
	 (*i)->accept(*this);
   }

   void acceptBlock(const Stmt::Block& block)
   {
      m_indent.beginBlock();
      acceptAll(block);
      m_indent.endBlock();
   }

public:
   TestVisitor(ostream& str, bool colorize = true) : 
      m_output(str), m_hlt(colorize)
   {
   }

   void visit(const Program& program)
   {
      acceptAll(program.stmts());
   }

   //Expression AST
   void visit(const Expr::Integer& intExpr)
   { 
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      ostringstream tmpStream;
      tmpStream << intExpr.value();
      m_output << m_hlt.integer(tmpStream.str());
      endExpr();
   }

   void visit(const Expr::Objnum& objnumExpr)
   { 
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      ostringstream tmpStream;
      tmpStream << objnumExpr.objnum();
      m_output << '#' << m_hlt.objnum(tmpStream.str());
      endExpr();
   }

   void visit(const Expr::Str& strExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.str(string("\"") + escapeStr(strExpr.str()) + '"'); 
      endExpr();
   }

   void visit(const Expr::Real& realExpr)
   { 
      beginExpr(PrecedenceInfo::PREC_PRIMARY);

      ostringstream tmpStream;
      tmpStream << realExpr.value();
      
      string realStr = tmpStream.str();
      if(realStr.find('.') == string::npos && 
	 realStr.find('e') == string::npos)
	 realStr += ".0";

      m_output << m_hlt.real(realStr);
      endExpr();
   }

   void visit(const Expr::List& listExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("{");
      listExpansion(listExpr.elements().begin(), listExpr.elements().end());
      m_output << m_hlt.symbol("}");
      endExpr();
   }

   void visit(const Expr::Variable& varExpr)
   { 
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.id(varExpr.id().name());
      endExpr();
   }

   void visit(const Expr::Scatter& scatterExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("{");
      listExpansion(scatterExpr.targets().begin(), 
		    scatterExpr.targets().end());
      m_output << m_hlt.symbol("}");
      endExpr();
   }

   void visit(const Expr::Scatter::Target& target)
   {
      typedef Expr::Scatter::Target Target;
    
      string id = m_hlt.id(target.id().name());

      switch(target.type()) {
	 case Target::REQUIRED:
	    m_output << id;
	    break;

	 case Target::OPTIONAL:
	    m_output << m_hlt.symbol("?") << id;
	    if(target.hasDefault()) {
	       m_output << ' ' << m_hlt.symbol("=") << ' ';
	       acceptNA(target.expr());
	    }
	    break;

	 case Target::REST:
	    m_output << m_hlt.symbol("@") << id;
	    break;
      }
   }

   void visit(const Expr::Not& expr)
   { preUnaryExpr("!", expr); }

   void visit(const Expr::Negate& expr)
   { preUnaryExpr("-", expr); }

   void visit(const Expr::Splice& expr)
   { preUnaryExpr("@", expr); }

   void visit(const Expr::PreInc& expr)
   { preUnaryExpr("++", expr); }

   void visit(const Expr::PreDec& expr)
   { preUnaryExpr("--", expr); }

   void visit(const Expr::PostInc& expr)
   { postUnaryExpr("++", expr); }

   void visit(const Expr::PostDec& expr)
   { postUnaryExpr("--", expr); }

   void visit(const Expr::Or& expr)
   { binaryExprLeft("||", expr, PrecedenceInfo::PREC_LOGICAL); }

   void visit(const Expr::And& expr)
   { binaryExprLeft("&&", expr, PrecedenceInfo::PREC_LOGICAL); }

   void visit(const Expr::Equal& expr) 
   { binaryExprLeft("==", expr, PrecedenceInfo::PREC_COMPARISON); }
		   
   void visit(const Expr::NotEqual& expr) 
   { binaryExprLeft("!=", expr, PrecedenceInfo::PREC_COMPARISON); }

   void visit(const Expr::Less& expr) 
   { binaryExprLeft("<", expr, PrecedenceInfo::PREC_COMPARISON); }

   void visit(const Expr::LessEqual& expr) 
   { binaryExprLeft("<=", expr, PrecedenceInfo::PREC_COMPARISON); }

   void visit(const Expr::GreaterEqual& expr) 
   { binaryExprLeft(">=", expr, PrecedenceInfo::PREC_COMPARISON); }

   void visit(const Expr::Greater& expr) 
   { binaryExprLeft(">", expr, PrecedenceInfo::PREC_COMPARISON); }

   void visit(const Expr::In& expr) 
   { 
      binaryExprLeft("in", 
		     expr, 
		     PrecedenceInfo::PREC_COMPARISON,
		     TestSyntaxHighlighter::RESERVED_WORD); 
   }

   void visit(const Expr::Add& expr) 
   { binaryExprLeft("+", expr, PrecedenceInfo::PREC_ADDITIVE); }

   void visit(const Expr::Sub& expr) 
   { binaryExprLeft("-", expr, PrecedenceInfo::PREC_ADDITIVE); }

   void visit(const Expr::Mul& expr) 
   { binaryExprLeft("*", expr, PrecedenceInfo::PREC_MULTIPLICATIVE); }

   void visit(const Expr::Div& expr)
   { binaryExprLeft("/", expr, PrecedenceInfo::PREC_MULTIPLICATIVE); }

   void visit(const Expr::Mod& expr) 
   { binaryExprLeft("%", expr, PrecedenceInfo::PREC_MULTIPLICATIVE); }
   
   void visit(const Expr::Exp& expr)
   { binaryExprRight("^", expr, PrecedenceInfo::PREC_EXPONENTIAL); }

   void visit(const Expr::Assign& expr) 
   { binaryExprNA("=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::AddEqual& expr) 
   { binaryExprNA("+=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::SubEqual& expr) 
   { binaryExprNA("-=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::MulEqual& expr) 
   { binaryExprNA("*=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::DivEqual& expr) 
   { binaryExprNA("/=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::ModEqual& expr) 
   { binaryExprNA("%=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::ExpEqual& expr) 
   { binaryExprNA("^=", expr, PrecedenceInfo::PREC_ASSIGN); }

   void visit(const Expr::Conditional& condExpr)
   {
      beginExpr(PrecedenceInfo::PREC_CONDITIONAL);
      acceptLT(condExpr.test());
      m_output << ' ' << m_hlt.symbol("?") << ' ';
      acceptNA(condExpr.trueExpr());
      m_output << ' ' << m_hlt.symbol("|") << ' ';
      acceptLE(condExpr.falseExpr());
      endExpr();
   }

   void visit(const Expr::Index& indexExpr)
   {
      beginExpr(PrecedenceInfo::PREC_POSTFIX);
      acceptLT(indexExpr.expr());
      m_output << m_hlt.symbol("[");
      acceptNA(indexExpr.index());
      m_output << m_hlt.symbol("]");
      endExpr();
   }

   void visit(const Expr::Range& rangeExpr)
   {
      beginExpr(PrecedenceInfo::PREC_POSTFIX);
      acceptLT(rangeExpr.expr());
      m_output << m_hlt.symbol("[");
      acceptNA(rangeExpr.start());
      m_output << m_hlt.symbol("..");
      acceptNA(rangeExpr.end());
      m_output << m_hlt.symbol("]");
      endExpr();
   }

   void visit(const Expr::Catch& catchExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("`");
      acceptNA(catchExpr.expr());
      m_output << ' ' << m_hlt.symbol("!") << ' ';
      
      if(!catchExpr.codes().empty())
	 listExpansion(catchExpr.codes().begin(), catchExpr.codes().end());
      else
	 m_output << m_hlt.id("ANY");

      if(catchExpr.hasExceptValue()) {
	 m_output << ' ' << m_hlt.symbol("=>") << ' ';
	 acceptNA(catchExpr.exceptValue());
      }

      m_output << m_hlt.symbol("'");
      endExpr();
   }

   void visit(const Expr::Prop& propExpr)
   {
      beginExpr(PrecedenceInfo::PREC_POSTFIX);
      acceptLT(propExpr.object());
      m_output << m_hlt.symbol(".");
      outputPropOrVerbName(propExpr.name());
      endExpr();
   }

   void visit(const Expr::SystemProp& sysPropExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("$") << m_hlt.id(sysPropExpr.name());
      endExpr();
   }

   void visit(const Expr::SystemCall& sysCallExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("$") << m_hlt.id(sysCallExpr.name()) 
	       << m_hlt.symbol("(");
      listExpansion(sysCallExpr.args().begin(), sysCallExpr.args().end());
      m_output << m_hlt.symbol(")");
      endExpr();
   }

   void visit(const Expr::VerbCall& verbCallExpr)
   {
      beginExpr(PrecedenceInfo::PREC_POSTFIX);
      acceptLT(verbCallExpr.object());
      m_output << m_hlt.symbol(":");
      outputPropOrVerbName(verbCallExpr.name());
      m_output << m_hlt.symbol("(");
      listExpansion(verbCallExpr.args().begin(), verbCallExpr.args().end());
      m_output << m_hlt.symbol(")");
      endExpr();
   }

   void visit(const Expr::Builtin& builtinExpr)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.id(builtinExpr.name()) << m_hlt.symbol("(");
      listExpansion(builtinExpr.args().begin(), builtinExpr.args().end());
      m_output << m_hlt.symbol(")");
      endExpr();
   }

   void visit(const Expr::Length&)
   {
      beginExpr(PrecedenceInfo::PREC_PRIMARY);
      m_output << m_hlt.symbol("$");
      endExpr();
   }

   //Statement AST
   void visit(const Stmt::If& ifStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("if") << ' ' << m_hlt.symbol("(");
      acceptNA(ifStmt.test());
      m_output << m_hlt.symbol(")") << '\n';
      acceptBlock(ifStmt.body());
      acceptAll(ifStmt.elseList());
      m_output << m_indent << m_hlt.reserved("endif") << '\n';
      endStmt();
   }

   void visit(const Stmt::If::Else& elseClause)
   {
      beginStmt();
      m_output << m_indent;

      if(elseClause.hasTest()) {
	 m_output << m_hlt.reserved("elseif") << ' ' << m_hlt.symbol("(");
	 acceptNA(elseClause.test());
	 m_output << m_hlt.symbol(")") << '\n';
      } else
	 m_output << m_hlt.reserved("else") << '\n';

      acceptBlock(elseClause.body());
      endStmt();
   }
	 
   void visit(const Stmt::While& whileStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("while") << ' ';

      if(whileStmt.id())
	 m_output << m_hlt.id(whileStmt.id().name()) << ' ';
					     
      m_output << m_hlt.symbol("(");
      acceptNA(whileStmt.test());
      m_output << m_hlt.symbol(")") << '\n';
      acceptBlock(whileStmt.body());
      m_output << m_indent << m_hlt.reserved("endwhile") << '\n';
      endStmt();
   }

   void visit(const Stmt::ForList& forStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("for") << ' ' 
	       << m_hlt.id(forStmt.id().name()) << ' '
	       << m_hlt.reserved("in") << ' ' << m_hlt.symbol("(");
      acceptNA(forStmt.expr());
      m_output << m_hlt.symbol(")") << '\n';
      acceptBlock(forStmt.body());
      m_output << m_indent << m_hlt.reserved("endfor") << '\n';
      endStmt();
   }

   void visit(const Stmt::ForRange& forStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("for") << ' ' 
	       << m_hlt.id(forStmt.id().name()) << ' '
	       << m_hlt.reserved("in") << " [";
      acceptNA(forStmt.start());
      m_output << m_hlt.symbol("..");
      acceptNA(forStmt.end());
      m_output << m_hlt.symbol("]") << '\n';
      acceptBlock(forStmt.body());
      m_output << m_indent << m_hlt.reserved("endfor") << '\n';
      endStmt();
   }

   void visit(const Stmt::TryExcept& tryStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("try") << '\n';
      acceptBlock(tryStmt.body());
      acceptAll(tryStmt.excepts());
      m_output << m_indent << m_hlt.reserved("endtry") << '\n';
      endStmt();
   }

   void visit(const Stmt::TryExcept::Except& except)
   {
      m_output << m_indent << m_hlt.reserved("except") << ' '
	       << (except.id() ? m_hlt.id(except.id().name()) + ' ' : "")
	       << m_hlt.symbol("(");
      listExpansion(except.codes().begin(), except.codes().end());
      m_output << m_hlt.symbol(")") << '\n';
      acceptBlock(except.body());
   }

   void visit(const Stmt::TryFinally& tryStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("try") << '\n';
      acceptBlock(tryStmt.body());
      m_output << m_indent << m_hlt.reserved("finally") << '\n';
      acceptBlock(tryStmt.finally());
      m_output << m_indent << m_hlt.reserved("endtry") << '\n';
      endStmt();
   }

   void visit(const Stmt::Fork& forkStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("fork") << ' '
	       << (forkStmt.id() ? m_hlt.id(forkStmt.id().name()) + ' ' : "")
	       << m_hlt.symbol("(");
      acceptNA(forkStmt.delay());
      m_output << m_hlt.symbol(")") << '\n';
      acceptBlock(forkStmt.body());
      m_output << m_indent << m_hlt.reserved("endfork") << '\n';
      endStmt();
   }

   void visit(const Stmt::Switch& switchStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("switch") << ' ' 
	       << m_hlt.symbol("(");
      acceptNA(switchStmt.expr());
      m_output << m_hlt.symbol(")") << '\n';
      m_indent.beginBlock();
      acceptAll(switchStmt.cases());
      m_indent.endBlock();
      m_output << m_indent << m_hlt.reserved("endswitch") << '\n';
      endStmt();
   }

   void visit(const Stmt::Switch::Case& caseClause)
   {
      m_output << m_indent;

      if(!caseClause.isDefault()) {
	 m_output << m_hlt.reserved("case") << ' ' << m_hlt.symbol("(");
	 acceptNA(caseClause.expr());
	 m_output << m_hlt.symbol(")") << '\n';
      } else
	 m_output << m_hlt.reserved("default") << '\n';

      acceptBlock(caseClause.body());
   }

   void visit(const Stmt::Return& returnStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("return");

      if(returnStmt.hasExpr()) {
	 m_output << ' ';
	 acceptNA(returnStmt.expr());
      }

      m_output << m_hlt.symbol(";") << '\n';
      endStmt();
   }

   void visit(const Stmt::Continue& continueStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("continue");

      if(continueStmt.id())
	 m_output << ' ' << m_hlt.id(continueStmt.id().name());

      m_output << m_hlt.symbol(";") << '\n';
      endStmt();
   }

   void visit(const Stmt::Break& breakStmt)
   {
      beginStmt();
      m_output << m_indent << m_hlt.reserved("break");

      if(breakStmt.id())
	 m_output << ' ' << m_hlt.id(breakStmt.id().name());

      m_output << m_hlt.symbol(";") << '\n';
      endStmt();
   }

   void visit(const Stmt::Expr& exprStmt)
   {
      beginStmt();
      m_output << m_indent;
      acceptNA(exprStmt.expr());
      m_output << m_hlt.symbol(";") << '\n';
      endStmt();
   }
};

int main(int argc, char** argv)
{
#ifdef CHECK_EXCEPT
   try {
#endif
   string source, line;
   
   if(argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[1], "-a"))) {
      cout << "Usage: " << argv[0] << " [-a] file\n";
      cout << "Options:\n";
      cout << setw(5) << "" << "-a" << setw(20) << ""
	   << "Turns on ANSI color escaping (default off)\n";
      return 1;
   } 
   
   ifstream input(argv[argc - 1]);
   if(!input) {
      cerr << "Error opening file" << endl;
      return 2;
   }
   
   while(getline(input, line))
      source += line + '\n';
   
   input.close();
      
   MyMessages msgs;
   Parser parser;
   if(parser.parse(source, msgs, true)) {
      std::auto_ptr<Program> program = parser.releaseProgram();
      TestVisitor visitor(cout, argc > 2);
      
      visitor.visit(*program);
   } else {
      return 1;
   }

#ifdef CHECK_EXCEPT
   } catch(const std::exception& e) {
      cerr << "Exception caught: " << e.what() << endl;
      return 2;
   }
#endif

#ifdef USE_DMALLOC
   dmalloc_shutdown();
#endif

   return 0;
}
   
