%{

#include "expr_ast.hpp"
#include "lexer.hpp"
#include "parser_msgs.hpp"
#include "parser_state.hpp"
#include "stmt_ast.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <functional>
#include <string>

#define MAKE_BINARY_EXPR(T, left, right) \
   addToPool(new Expr::T(poolToAuto(left), \
			 poolToAuto(right)))

#define BEGIN_BLOCK() do {
#define END_BLOCK() } while(0)

namespace {

using namespace std;
using namespace Moove;

ParserState* parser;

/**
 * \internal
 * \brief Simplifies adding allocated objects to memory pool
 * \param ptr Pointer to add to pool and return
 *
 * Allows assigning to $$ and adding to memory pool in one line. Only
 * used for objects descended from MemoryPoolObject.
 */
template<class T>
inline T* addToPool(T* ptr)
{
   parser->addToPool(auto_ptr<ASTPoolObject>(ptr));
   return ptr;
}

/*
template<class T>
MemoryPoolPointer<T>* addPtrToPool(T* ptr)
{
   MemoryPoolPointer<T>* mpPtr = new MemoryPoolPointer<T>(auto_ptr<T>(ptr));
   parser->addToPool(auto_ptr<MemoryPoolObject>(mpPtr));
   return mpPtr;
}
*/

template<class T>
inline auto_ptr<T> poolToAuto(T* ptr)
{
   parser->removeFromPool(ptr);
   return auto_ptr<T>(ptr);
}

template<class T>
auto_ptr<T> poolPtrToAuto(ASTPoolPtr<T>* ptr)
{
   if(ptr) {
      parser->removeFromPool(ptr);
      auto_ptr<T> tmp(*ptr);
      delete ptr;
      return tmp;
   } else
      return auto_ptr<T>(0);
}

/**
 * \internal
 * \brief Calls cont.push_back(ptr) with exception safety
 * \param cont Container to insert into
 * \param ptr Pointer to a deletable object
 * 
 * If an exception is thrown while inserting \a ptr into \a cont, \a ptr
 * is deleted.
 */
template<class Container, class T>
void safePushBack(ASTAutoContainer<Container>& cont, T* ptr)
{
   auto_ptr<T> tmp(ptr);
   cont.push_back(ptr);
   tmp.release();
}

template<class Container, class T>
void safePushBack(ASTAutoContainer<Container>& cont, auto_ptr<T> ptr)
{
   cont.push_back(ptr.get());
   ptr.release();
}

inline void yyerror(const std::string& msg)
{
   parser->error(msg);
}

int yylex();

typedef Expr::Scatter::Target       ScatterTarget;
typedef Expr::Scatter::TargetList   ScatterTargetList;
typedef Stmt::TryExcept::Except     Except;
typedef Stmt::TryExcept::ExceptList ExceptList;

bool listToScatter(ScatterTargetList& targets, const Expr::ArgList& list);

%}

%union
{
   int                                    integer;
   Moove::ASTPoolPtr<std::string>* str;
   Moove::ASTPoolPtr<double>*      real;

   Moove::Expr::ArgList*                  list;
   Moove::Expr::Expr*                     expr;
   Moove::Expr::Scatter::Target*          target;
   Moove::Expr::Scatter::TargetList*      targetList;

   Moove::Stmt::Stmt*                     stmt;
   Moove::Stmt::Block*                    stmts;
   Moove::Stmt::If::ElseList*             elseList;
   Moove::Stmt::TryExcept::ExceptList*    exceptList;
   Moove::Stmt::Switch::CaseList*         caseList;
}

%type <stmts>      stmts else_block
%type <stmt>       stmt 
%type <expr>       expr dollars_up catch_result argument
%type <elseList>      elseifs
%type <str>         opt_id
%type <list>       arglist ne_arglist except_codes
%type <targetList> scatter
%type <target>     opt_target
%type <exceptList>    excepts
%type <caseList>      switch_cases

%token <integer> tINT tOBJNUM
%token <str>     tSTR tID
%token <real>    tREAL

%token tIF tELSEIF tELSE tENDIF
%token tWHILE tENDWHILE
%token tFOR tENDFOR
%token tFORK tENDFORK
%token tTRY tEXCEPT tFINALLY tENDTRY
%token tSWITCH tCASE tDEFAULT tENDSWITCH
%token tRETURN tCONTINUE tBREAK
%token tANY tTO tARROW tIN

%right    '=' tADD_EQ tSUB_EQ tMUL_EQ tDIV_EQ tMOD_EQ tEXP_EQ
%nonassoc '?' '|'
%left     tOR tAND
%left     tEQ tNE '<' tLE '>' tGE tIN
%left     '+' '-'
%left     '*' '/' '%'
%right    '^'
%left     '!' tNEGATE tINCREMENT tDECREMENT
%nonassoc '.' ':' '[' '$'

%%

program:             stmts
                     {
			parser->setProgram(poolToAuto($1));
                     }
    ;

stmts:               /* nothing */
                     {
			$$ = addToPool(new Stmt::Block);
                     }
                |    stmts stmt
                     {
			$$ = $1;

			if($2)
			   safePushBack(*$$, poolToAuto($2));

                     }
    ;

stmt:                tIF '(' expr ')' stmts elseifs else_block tENDIF
                     {
			if($7)
			   safePushBack(*$6, new Stmt::If::Else(poolToAuto($7)));

                        $$ = addToPool(new Stmt::If(poolToAuto($3), 
						    poolToAuto($5), 
						    poolToAuto($6)));
                     }
                |    tWHILE opt_id '(' expr ')'
                     {
                        parser->beginLoop(poolPtrToAuto($2).get());
                     }
                         stmts tENDWHILE
                         {
                            $$ = addToPool(new Stmt::While(parser->loopID(), 
							   poolToAuto($4), 
							   poolToAuto($7)));
                            parser->endLoop();
                         }
                |    tFOR tID tIN '(' expr ')'
                     {
			parser->beginLoop(poolPtrToAuto($2).get());
                     }
                         stmts tENDFOR
                         {
			    $$ = addToPool(new Stmt::ForList(parser->loopID(), 
							     poolToAuto($5), 
							     poolToAuto($8)));
			    parser->endLoop();
                         }
                |    tFOR tID tIN '[' expr tTO expr ']'
                     {
                        parser->beginLoop(poolPtrToAuto($2).get());
                     }
                         stmts tENDFOR
                         {
			    $$ = addToPool(new Stmt::ForRange(parser->loopID(),
							      poolToAuto($5), 
							      poolToAuto($7), 
							      poolToAuto($10)));
			    parser->endLoop();
                         }
                |    tTRY stmts excepts tENDTRY
                     {
			$$ = addToPool(new Stmt::TryExcept(poolToAuto($2), 
							   poolToAuto($3)));
                     }
                |    tTRY stmts tFINALLY stmts tENDTRY
                     {
			$$ = addToPool(new Stmt::TryFinally(poolToAuto($2), 
							    poolToAuto($4)));
                     }
                |    tFORK opt_id '(' expr ')'
		     {
			parser->beginFork();

			if($2)
			   parser->addVar(**$2);
		     }   
			 stmts tENDFORK
			 {
			    BEGIN_BLOCK();
			    auto_ptr<string> id = poolPtrToAuto($2);
			    Symbol forkVar = (id.get() ? 
					      parser->findVar(*id) : 
					      Symbol());
			    $$ = addToPool(new Stmt::Fork(forkVar,
							  poolToAuto($4), 
							  poolToAuto($7)));
			    parser->endFork();
			    END_BLOCK();
			 }
                |    tSWITCH '(' expr ')' 
                     {
			parser->beginSwitch();
		     }
			 switch_cases tENDSWITCH
			 {
			    $$ = addToPool(new Stmt::Switch(poolToAuto($3), 
							    poolToAuto($6)));
			    parser->endSwitch();
			 }
                |    tRETURN ';'
                     {
			$$ = addToPool(new Stmt::Return);
		     }
                |    tRETURN expr ';'
                     {
                        $$ = addToPool(new Stmt::Return(poolToAuto($2)));
                     }
                |    tCONTINUE opt_id ';'
                     {
			BEGIN_BLOCK();
			std::auto_ptr<std::string> id = poolPtrToAuto($2);
			Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

			if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
			   if(id.get())
			      yyerror("Invalid loop name in `continue' "
				      "statement: " + *id);
			   else
			      yyerror("No enclosing loop or switch for "
				      "`continue' statement");
			}

			$$ = addToPool(new Stmt::Continue(sym));
			END_BLOCK();
                     }
                |    tBREAK opt_id ';'
                     {
			BEGIN_BLOCK();
			std::auto_ptr<std::string> id = poolPtrToAuto($2);
			Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

			if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
			   if(id.get())
			      yyerror("Invalid loop name in `break' "
				      "statement: " + *id);
			   else
			      yyerror("No enclosing loop or switch for "
				      "`break' statement");
			}

			$$ = addToPool(new Stmt::Break(sym));
			END_BLOCK();
                     }
                |    expr ';' 
                     {
			$$ = addToPool(new Stmt::Expr(poolToAuto($1)));
                     }
                |    ';'
                     {
                        $$ = 0;
                     }
    ;

elseifs:             /* nothing */
                     {
                        $$ = addToPool(new Stmt::If::ElseList);
                     }
                |    elseifs tELSEIF '(' expr ')' stmts
                     {
			$$ = $1;
			safePushBack(*$$, new Stmt::If::ElseIf(poolToAuto($4), 
							       poolToAuto($6)));
                     }
    ;

else_block:          /* nothing */
                     {
                        $$ = 0;
                     }
                |    tELSE stmts
                     {
                        $$ = $2;
                     }
    ;

opt_id:              /* nothing */
                     {
                        $$ = 0;
                     }
                |    tID
                     {
                        $$ = $1;
                     }
    ;

excepts:             tEXCEPT opt_id '(' except_codes ')' stmts
                     {
			BEGIN_BLOCK();
                        $$ = addToPool(new ExceptList);
			Symbol var = ($2 ? 
				      parser->addVar(*poolPtrToAuto($2)) : 
				      Symbol());
			safePushBack(*$$, new Except(var,
						     poolToAuto($4),
						     poolToAuto($6)));
			END_BLOCK();
                     }
                |    excepts tEXCEPT opt_id '(' except_codes ')' stmts
                     {
			BEGIN_BLOCK();
			if($1->back()->codes().empty())
                           yyerror("Unreachable EXCEPT clause");
			
			$$ = $1;
			Symbol var = ($3 ? 
				      parser->addVar(*poolPtrToAuto($3)) :
				      Symbol());
			safePushBack(*$$, new Except(var,
						     poolToAuto($5),
						     poolToAuto($7)));
			END_BLOCK();
		     }
    ;

except_codes:        tANY
                     {
                        $$ = addToPool(new Expr::ArgList);
                     }
                |    ne_arglist
                     {
                        $$ = $1;
                     }
    ;

switch_cases:        /* nothing */
                     {
			$$ = addToPool(new Stmt::Switch::CaseList);
		     }
                |    switch_cases tCASE '(' expr ')' stmts
		     {
			$$ = $1;
			safePushBack(*$$, 
				     new Stmt::Switch::Case(poolToAuto($4), 
							    poolToAuto($6)));
		     }
                |    switch_cases tDEFAULT 
		     {
			if(find_if($1->begin(), $1->end(),
				   mem_fun(&Stmt::Switch::Case::isDefault)) != $1->end())
			   yyerror("Multiple default clauses in switch statement");
		     }
                         stmts
                         {
			    $$ = $1;
			    safePushBack(*$$, 
					 new Stmt::Switch::Default(poolToAuto($4)));
			 }
    ;

arglist:             /* nothing */
                     {
			$$ = addToPool(new Expr::ArgList);
                     }
                |    ne_arglist
                     {
                        $$ = $1;
                     }
    ;

ne_arglist:          argument
                     {
			$$ = addToPool(new Expr::ArgList);
			safePushBack(*$$, poolToAuto($1));
                     }
                |    ne_arglist ',' argument
                     {
			$$ = $1;
			safePushBack(*$$, poolToAuto($3));
                     }
    ;

argument:            expr
                     {
			$$ = $1;
                     }
                |    '@' expr
                     {
			$$ = addToPool(new Expr::Splice(poolToAuto($2)));
                     }
    ;


expr:                tINT
                     {
                        $$ = addToPool(new Expr::Integer($1));
                     }
                |    tOBJNUM
                     {
			$$ = addToPool(new Expr::Objnum($1));
		     }
                |    tSTR
                     {
			$$ = addToPool(new Expr::Str(poolPtrToAuto($1)));
                     }
                |    tREAL
                     {
			$$ = addToPool(new Expr::Real(poolPtrToAuto($1)));
                     }
                |    '{' arglist '}'
                     {
			$$ = addToPool(new Expr::List(poolToAuto($2)));
                     }
                |    tID
                     {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto($1));
			$$ = addToPool(new Expr::Variable(var));
			END_BLOCK();
                     }
                |    '!' expr
                     {
			$$ = addToPool(new Expr::Not(poolToAuto($2)));
                     }
                |    '-' expr   %prec tNEGATE
                     {
			$$ = addToPool(new Expr::Negate(poolToAuto($2)));
                     }
                |    tINCREMENT expr
                     {
			if(!$2->assignable())
			   yyerror("Invalid operand to ++ operator");

			$$ = addToPool(new Expr::PreInc(poolToAuto($2)));
                     }
                |    tDECREMENT expr
                     {
			if(!$2->assignable())
			   yyerror("Invalid operand to -- operator");

			$$ = addToPool(new Expr::PreDec(poolToAuto($2)));
                     }
                |    expr tINCREMENT
                     {
			if(!$1->assignable())
			   yyerror("Invalid operand to ++ operator");

			$$ = addToPool(new Expr::PostInc(poolToAuto($1)));
                     }
                |    expr tDECREMENT
                     
		     {
			if(!$1->assignable())
			   yyerror("Invalid operand to -- operator");

			$$ = addToPool(new Expr::PostDec(poolToAuto($1)));

                     }
                |    expr '=' expr
                     {
			BEGIN_BLOCK();
			std::auto_ptr<Expr::Expr> dest = poolToAuto($1);

			if(!dest->assignable()) {
			   bool converted = false;
			   Expr::List* list;

			   if((list = dynamic_cast<Expr::List*>(dest.get()))) {
			      std::auto_ptr<ScatterTargetList> targets(new ScatterTargetList);
			      converted = listToScatter(*targets, 
							list->elements());
			      dest.reset(new Expr::Scatter(targets));
			   } 
			   
			   if(!converted)
			      yyerror("Invalid expression on left side of assignment");
			}

                        $$ = addToPool(new Expr::Assign(dest, poolToAuto($3)));
			END_BLOCK();
                     }
                |    '{' scatter '}' '=' expr
                     {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> scatter(new Expr::Scatter(poolToAuto($2)));
			$$ = addToPool(new Expr::Assign(scatter,
							poolToAuto($5)));
			END_BLOCK();
                     }
                |    expr tOR expr
                     {
			$$ = MAKE_BINARY_EXPR(Or, $1, $3);
                     }
                |    expr tAND expr
                     {
			$$ = MAKE_BINARY_EXPR(And, $1, $3);
                     }
                |    expr tEQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(Equal, $1, $3);
                     }
                |    expr tNE expr
                     {
                        $$ = MAKE_BINARY_EXPR(NotEqual, $1, $3);
                     }
                |    expr '<' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Less, $1, $3);
                     }
                |    expr tLE expr
                     {
                        $$ = MAKE_BINARY_EXPR(LessEqual, $1, $3);
                     }
                |    expr '>' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Greater, $1, $3);
                     }
                |    expr tGE expr
                     {
                        $$ = MAKE_BINARY_EXPR(GreaterEqual, $1, $3);
                     }
                |    expr tIN expr
                     {
                        $$ = MAKE_BINARY_EXPR(In, $1, $3);
                     }
                |    expr '+' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Add, $1, $3);
                     }
                |    expr '-' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Sub, $1, $3);
                     }
                |    expr '*' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Mul, $1, $3);
                     }
                |    expr '/' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Div, $1, $3);
                     }
                |    expr '%' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Mod, $1, $3);
                     }
                |    expr '^' expr
                     {
                        $$ = MAKE_BINARY_EXPR(Exp, $1, $3);
                     }
                |    expr tADD_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(AddEqual, $1, $3);
                     }
                |    expr tSUB_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(SubEqual, $1, $3);
                     }
                |    expr tMUL_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(MulEqual, $1, $3);
                     }
                |    expr tDIV_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(DivEqual, $1, $3);
                     }
                |    expr tMOD_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(ModEqual, $1, $3);
                     }
                |    expr tEXP_EQ expr
                     {
                        $$ = MAKE_BINARY_EXPR(ExpEqual, $1, $3);
                     }
                |    expr '?' expr '|' expr
                     {
			$$ = addToPool(new Expr::Conditional(poolToAuto($1), 
							     poolToAuto($3), 
							     poolToAuto($5)));
                     }
                |    expr '.' tID
                     {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto($3)));
			$$ = addToPool(new Expr::Prop(poolToAuto($1),
						      name));
			END_BLOCK();
                     }
                |    expr '.' '(' expr ')'
                     {
			$$ = addToPool(new Expr::Prop(poolToAuto($1), 
						      poolToAuto($4)));
                     }
                |    expr ':' tID '(' arglist ')'
                     {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto($3)));
			$$ = addToPool(new Expr::VerbCall(poolToAuto($1), 
							  name, 
							  poolToAuto($5)));
			END_BLOCK();
		     }
                |    expr ':' '(' expr ')' '(' arglist ')'
                     {
			$$ = addToPool(new Expr::VerbCall(poolToAuto($1),
							  poolToAuto($4),
							  poolToAuto($7)));
		     }
                |    expr '[' dollars_up expr ']'
                     {
			parser->decDollarDepth();
			$$ = addToPool(new Expr::Index(poolToAuto($1),
						       poolToAuto($4)));
		     }
                |    expr '[' dollars_up expr tTO expr ']'
                     {
			parser->decDollarDepth();
			$$ = addToPool(new Expr::Range(poolToAuto($1), 
						       poolToAuto($4), 
						       poolToAuto($6)));
		     }
                |    '$'
                     {
			if(!parser->dollarDepth())
			   yyerror("Illegal context for `$' expression");

			$$ = addToPool(new Expr::Length);
		     }
                |    '$' tID
                     {
			$$ = addToPool(new Expr::SystemProp(poolPtrToAuto($2)));
		     }
                |    '$' tID '(' arglist ')'
                     {
			$$ = addToPool(new Expr::SystemCall(poolPtrToAuto($2),
							    poolToAuto($4)));
		     }
                |    tID '(' arglist ')'
                     {
			$$ = addToPool(new Expr::Builtin(poolPtrToAuto($1), 
							 poolToAuto($3)));
		     }
                |    '`' expr '!' except_codes catch_result '\''
                     {
			$$ = addToPool(new Expr::Catch(poolToAuto($2), 
						       poolToAuto($4),
						       poolToAuto($5)));
		     }
                |    '(' expr ')'
                     {
			 $$ = $2;
		     }
    ;


scatter:             opt_target
                     {
			$$ = addToPool(new ScatterTargetList);
			safePushBack(*$$, poolToAuto($1));
		     }
                |    scatter ',' opt_target
                     {
			$$ = $1;
			safePushBack(*$$, poolToAuto($3));
		     }
                |    scatter ',' tID
                     {
			BEGIN_BLOCK();
			$$ = $1;
			Symbol var = parser->addVar(*poolPtrToAuto($3));
			safePushBack(*$$, 
				     new ScatterTarget(ScatterTarget::REQUIRED, var));
			END_BLOCK();
		     }
                |    scatter ',' '@' tID
                     {
			BEGIN_BLOCK();
			$$ = $1;
			Symbol var = parser->addVar(*poolPtrToAuto($4));
			safePushBack(*$$, 
				     new ScatterTarget(ScatterTarget::REST, var));
			END_BLOCK();
		     }
                |    ne_arglist ',' opt_target
                     {
			$$ = addToPool(new ScatterTargetList);
			listToScatter(*$$, *poolToAuto($1));
			safePushBack(*$$, poolToAuto($3));
		     }
    ;

opt_target:          '?' tID
                     {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto($2));
			$$ = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
							 var));
			END_BLOCK();
		     }
                |    '?' tID '=' expr
                     {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto($2));
			$$ = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
							 var,
							 poolToAuto($4)));
			END_BLOCK();
		     }
    ;

dollars_up:          /* nothing */
                     {
			parser->incDollarDepth();
		     }
    ;

catch_result:        /* nothing */
                     {
			$$ = 0;
		     }
                |    tARROW expr
                     {
			$$ = $2;
		     }
    ;

%%
 
}   //namespace

#include "lexer_symbols.hpp"

namespace {

Symbol isRestTarget(const Expr::Expr* expr)
{
   const Expr::Splice* splice;
   const Expr::Variable* var;

   if((splice = dynamic_cast<const Expr::Splice*>(expr)) &&
      (var = dynamic_cast<const Expr::Variable*>(&splice->operand())))
      return var->id();
   else
      return Symbol();
}
   
bool listToScatter(ScatterTargetList& targets, const Expr::ArgList& list)
{
   typedef Expr::ArgList ArgList;
   typedef Expr::Variable Variable;
   typedef Expr::Splice Splice;
   typedef ScatterTarget Target;

   bool success = true;

   ArgList::const_iterator end = list.end();
   for(ArgList::const_iterator item = list.begin(); item != end; ++item) {
      Symbol id;

      if(const Variable* var = dynamic_cast<const Variable*>(*item)) {
	 std::auto_ptr<Target> target(new ScatterTarget(Target::REQUIRED,
							var->id()));
	 targets.push_back(target.get());
	 target.release();
      } else if((id = isRestTarget(*item))) {
	 std::auto_ptr<Target> target(new ScatterTarget(Target::REST,
							id));
	 targets.push_back(target.get());
	 target.release();
      } else {
	 yyerror("Scattering assignment targets must be simple variables.");
	 success = false;
      }
   }

   return success;
}

void checkAssignable(const Expr::Expr& expr, const char* assignType)
{
   if(!expr.assignable())
      yyerror(string("Illegal expression on left side of ") +
	      assignType + '.');
}

int yylex()
{
   return parser->lexer().nextToken();
}

}    //namespace

namespace Moove {

void Lexer::error(const string& msg)
{
   m_messages.error(msg, m_line);
}

void Lexer::warning(const string& msg)
{
   m_messages.warning(msg, m_line);
}

bool Lexer::isWS(char ch)
{
   return ch == ' ' || ch == '\t' || ch == '\r';
}

bool Lexer::myIsDigit(char ch)
{
   return isdigit(ch);
}

bool Lexer::isIDSuffixChar(char ch)
{ 
   return isalnum(ch) || ch == '_';
}

void Lexer::skipWS()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&isWS)));
}

void Lexer::skipDigits()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&myIsDigit)));
}

void Lexer::skipID()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&isIDSuffixChar)));
}

int Lexer::parseNumber(bool realOK)
{
   string::const_iterator start = m_pos;
   
   //integer portion
   skipDigits();

   char ch = peekCur();
   if(realOK && 
      ((ch == '.' && peekNext() != '.') || ch == 'e' || ch == 'E')) {
      //fractional portion
      if(ch == '.') {
	 ++m_pos;
	 skipDigits();
	 ch = peekCur();
      }

      //exponent portion
      if(ch == 'e' || ch == 'E') {
	 ++m_pos;
	 ch = peekCur();
	 if(ch == '-' || ch == '+')
	    ++m_pos;
	 
	 if(isdigit(peekCur()))
	    skipDigits();
	 else {
	    error("Malformed floating-point literal");
	    return tINT;
	 }
      }

      double value = strtod(string(start, m_pos).c_str(), 0);
      if(errno != ERANGE) {
	 yylval.real = addToPool(new ASTPoolPtr<double>(new double(value)));
	 return tREAL;
      } else {
	 error("Real value out of range");
	 return tINT;
      }
   } else {
      long value = strtol(string(start, m_pos).c_str(), 0, 10);
      if(errno != ERANGE && static_cast<int>(value) == value) {
	 yylval.integer = value;
	 return tINT;
      } else {
	 error("Integer literal out of range");
	 return tINT;
      }
   }
}   

int Lexer::parseObjnum()
{
   bool negative = false;

   ++m_pos;
   if(peekCur() == '-') {
      negative = true;
      ++m_pos;
   }
   
   int ret = parseNumber(false) != -1 ? tOBJNUM : -1;
   
   if(negative && ret == tOBJNUM)
      yylval.integer = -yylval.integer;

   return ret;
}

int Lexer::parseString()
{
   yylval.str = addToPool(new ASTPoolPtr<string>(new string));
   
   for(string::const_iterator start = ++m_pos; m_pos != m_end; ++m_pos) {
      if(*m_pos == '"') {
	 (*yylval.str)->append(start, m_pos);
	 ++m_pos;
	 return tSTR;
      } else if(*m_pos == '\\') {
	 if(++m_pos != m_end && *m_pos != '\n') {
	    (*yylval.str)->append(start, m_pos - 1);
	    **yylval.str += *m_pos;
	    start = m_pos + 1;
	 } else
	    break;
      } else if(*m_pos == '\n')
	 break;
   }

   error("Missing quote");
   if(m_pos != m_end)
      ++m_pos;
   
   return tSTR;
}

void Lexer::parseComment()
{
   const char* commentEnd = "*/";
	    
   string::const_iterator pos = search(m_pos + 2, m_end,
				       commentEnd, commentEnd + 2);
   if(pos == m_end)
      error("End of program while in a comment");
   else
      m_pos = pos + 2;
}

int Lexer::parseID()
{
   string::const_iterator start = m_pos++;
   skipID();

   auto_ptr<string> id(new string(start, m_pos));
	 
   //Check ID in reserved word set
   const LexerSymbol* sym = LexerSymbolTable::in_word_set(id->c_str(),
							  id->size());
   if(sym) {
      //ID is a reserved word. Return appropriate token
      return sym->token;
   } else {
      //ID is not a reserved word
      yylval.str = addToPool(new ASTPoolPtr<string>(id));
      return tID;
   }
}

int Lexer::parseOp()
{
   if(m_end - m_pos >= 2) {
      //First check the next 2 characters for a 2-char operator stored
      //in the LexerSymbolTable
      string op(m_pos, m_pos + 2);
      const LexerSymbol* sym = LexerSymbolTable::in_word_set(op.c_str(),
							  op.size());
      if(sym) {
	 //It is a 2-char operator. Return appropriate token
	 m_pos += 2;
	 return sym->token;
      }
   }
    
   //Assume character is a 1-char operator
   return *m_pos++;
}

Lexer::Lexer(const std::string& str, ParserMessages& msgs, bool enableObjnums) : 
   m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
   m_line(1), m_messages(msgs), m_enableObjnums(enableObjnums)
{}

Lexer::Lexer(const char* str, ParserMessages& msgs, bool enableObjnums) :
   m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
   m_line(1), m_messages(msgs), m_enableObjnums(enableObjnums)
{}

int Lexer::nextToken()
{
   skipWS();

   while(m_pos != m_end) {
      char ch = *m_pos;

      if(ch == '\n') {
	 ++m_line;
	 ++m_pos;
	 skipWS();
      } else if(ch == '/' && peekNext() == '*') {
	 parseComment();
      } else if(ch == '"') {
	 return parseString();
      } else if(isdigit(ch) || (ch == '.' && isdigit(peekNext()))) {
	 return parseNumber(true);
      } else if(m_enableObjnums && ch == '#') {
	 return parseObjnum();
      } else if(isalpha(ch) || ch == '_') {
	 return parseID();
      } else
	 return parseOp();
   }
      
   return -1;
}

/**
 * \brief Type used for communication with parser sub-system
 *
 * An instance of this type will both provide a method of communication
 * with the calling Compiler object as well as hold parsing state
 * information such as loop depth, variables, and constants.
 */
int parseSource(ParserState& newParser)
{
   parser = &newParser;
   int ret = yyparse();
   newParser.parseFinished();
   return ret;
}

}   //namespace Moove
