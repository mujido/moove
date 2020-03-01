%require "3.4"
%language "c++"
%defines
%define api.namespace {Moove::BisonParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%skeleton "lalr1.cc"
%param { ParserState& parserState }

%code requires {

#include "expr_ast.hpp"
#include "stmt_ast.hpp"

#include <boost/optional.hpp>

namespace Moove
{
    class ParserState;
}

}

%code {

#include "lexer.hpp"
#include "parser_msgs.hpp"
#include "parser_state.hpp"

#include <algorithm>

namespace Moove {
	typedef Expr::Scatter::Target       ScatterTarget;
	typedef Expr::Scatter::TargetList   ScatterTargetList;
	typedef Stmt::TryExcept::Except     Except;
	typedef Stmt::TryExcept::ExceptList ExceptList;

	bool listToScatter(ScatterTargetList& targets, const Expr::ArgList& list);

    namespace BisonParser {
		parser::symbol_type yylex(ParserState& parserState);
    }
}

} // code

%token END 0 "end of input"

%token <int> tINT tOBJNUM
%token <::std::string>     tSTR tID
%token <double>    tREAL

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

%start program;
program:            stmts
                    {
                        parserState.setProgram(std::move($1));
                    }
     ;

%nterm <Stmt::Block> stmts;
stmts:              %empty
                    {
                        // empty block
                    }
               |    stmts stmt
                    {
                        $$ = std::move($1);

                        if($2)
                            $$.push_back(std::move($2));
                    }
     ;

%nterm <std::unique_ptr<Stmt::Stmt>> stmt;
stmt:               tIF '(' expr ')' stmts elseifs else_block tENDIF
                    {
                        if($7)
                            $6.push_back(std::move(*$7));

                        $$ = std::make_unique<Stmt::If>(std::move($3), std::move($5), std::move($6));
                    }
               |    tWHILE opt_id '(' expr ')'
                    {
                        parserState.beginLoop($2);
                    }
                        stmts tENDWHILE
                        {
                            $$ = std::make_unique<Stmt::While>(parserState.loopID(), std::move($4), std::move($7));
                            parserState.endLoop();
                        }
               |    tFOR tID tIN '(' expr ')'
                    {
                        parserState.beginLoop($2);
                    }
                        stmts tENDFOR
                        {
                            $$ = std::make_unique<Stmt::ForList>(parserState.loopID(), std::move($5), std::move($8)); 
                            parserState.endLoop();
                        }
                |   tFOR tID tIN '[' expr tTO expr ']'
                    {
                        parserState.beginLoop($2);
                    }
                        stmts tENDFOR
                        {
                            $$ = std::make_unique<Stmt::ForRange>(
                                parserState.loopID(), std::move($5), std::move($7), std::move($10)
                            );
                            parserState.endLoop();
                        }
                |   tTRY stmts excepts tENDTRY
                    {
                        $$ = std::make_unique<Stmt::TryExcept>(std::move($2), std::move($3));
                    }
                |   tTRY stmts tFINALLY stmts tENDTRY
                    {
                        $$ = std::make_unique<Stmt::TryFinally>(std::move($2), std::move($4));
                    }
                |   tFORK opt_id '(' expr ')'
                    {
                        parserState.beginFork();

                        if($2)
                            parserState.addVar(*$2);
                    }   
                        stmts tENDFORK
                        {
                            Symbol forkVar = $2 ? parserState.findVar(*$2) : Symbol();
                            $$ = std::make_unique<Stmt::Fork>(forkVar, std::move($4), std::move($7));
                            parserState.endFork();
                   }
                |  tSWITCH '(' expr ')' 
                   {
                       parserState.beginSwitch();
                   }
                        switch_cases tENDSWITCH
                        {
                            $$ = std::make_unique<Stmt::Switch>(std::move($3), std::move($6));
                            parserState.endSwitch();
                        }
                |   tRETURN ';'
                    {
                        $$ = std::make_unique<Stmt::Return>();
                    }
                |   tRETURN expr ';'
                    {
                        $$ = std::make_unique<Stmt::Return>(std::move($2));
                    }
                |   tCONTINUE opt_id ';'
                    {
                        Symbol sym = $2 ? parserState.findVar(*$2) : Symbol();

                        if(!parserState.checkLoopID(sym) && !parserState.switchDepth()) {
                            if($2)
                                throw parser::syntax_error("Invalid loop name in `continue' statement: " + *$2);
                            else
                                throw parser::syntax_error("No enclosing loop or switch for `continue' statement");
                        }

                        $$ = std::make_unique<Stmt::Continue>(sym);
                    }
                |   tBREAK opt_id ';'
                    {
                        Symbol sym = $2 ? parserState.findVar(*$2) : Symbol();

                        if(!parserState.checkLoopID(sym) && !parserState.switchDepth()) {
                            if($2)
                                throw parser::syntax_error("Invalid loop name in `break' statement: " + *$2);
                            else
                                throw parser::syntax_error("No enclosing loop or switch for `break' statement");
                        }

                        $$ = std::make_unique<Stmt::Break>(sym);
                    }
                |   expr ';'
                    {
                        $$ = std::make_unique<Stmt::ExprStmt>(std::move($1));
                    }
                |   ';'
                    {
                        /* null ptr */
                    }
    ;

%nterm <Stmt::If::ElseList> elseifs;
elseifs:            %empty
                    {
                        // empty elseif block
                    }
               |    elseifs tELSEIF '(' expr ')' stmts
                    {
                        $$ = std::move($1);
                        $$.push_back(Stmt::If::ElseIf{ std::move($4), std::move($6) });
                    }
 ;

%nterm <boost::optional<Stmt::If::Else>> else_block;
else_block:         %empty
                    {
                        $$ = boost::none;
                    }
               |    tELSE stmts
					{
						$$ = std::move($2);
					}
     ;

%nterm <boost::optional<std::string>> opt_id;
opt_id:              %empty
                     {
                        $$ = boost::none;
                     }
                |    tID
                     {
                        $$ = std::move($1);
                     }
    ;

%nterm <Stmt::TryExcept::ExceptList> excepts;
excepts:            tEXCEPT opt_id '(' except_codes ')' stmts
                    {
                        Symbol var = $2 ? parserState.addVar(*$2) : Symbol();
                        $$.emplace_back(var, std::move($4), std::move($6));
                    }
                |   excepts tEXCEPT opt_id '(' except_codes ')' stmts
                    {
                        if($1.back().codes().empty())
                            throw parser::syntax_error("Unreachable EXCEPT clause");
                           
                        $$ = std::move($1);
                        Symbol var = $3 ? parserState.addVar(*$3) : Symbol();
                        $$.emplace_back(var, std::move($5), std::move($7));
                    }
     ;

%nterm <Stmt::Switch::CaseList> switch_cases;
switch_cases:       %empty
                    {
                       // empty list 
                    }
                |   switch_cases tCASE '(' expr ')' stmts
                    {
                        $$ = std::move($1);
                        $$.emplace_back(std::move($4), std::move($6));
                    }
                |   switch_cases tDEFAULT 
                    {
                        auto defaultCase = std::find_if($1.begin(), $1.end(), [](const auto& caseStmt) 
                        { 
                            return caseStmt.isDefault(); 
                        });
                        if (defaultCase != $1.end())
                            throw parser::syntax_error("Multiple default clauses in switch statement");
                    }
                        stmts
                        {
                            $$ = std::move($1);
                            $$.emplace_back(std::move($4));
                        }
     ;

%nterm <Expr::ArgList> except_codes;
except_codes:       tANY
                    {
                        /* empty list */
                    }
                |   ne_arglist
                    {
                        $$ = std::move($1);
                    }
     ;

%nterm <Expr::ArgList> arglist;
arglist:            %empty
                    {
                        // empty list
                    }
                |   ne_arglist
                    {
                        $$ = std::move($1);
                    }
     ;
     
%nterm <Expr::ArgList> ne_arglist;
ne_arglist:         argument
                    {
                        $$.push_back(std::move($1));
                    }
                |   ne_arglist ',' argument
                    {
                        $$ = std::move($1);
                        $$.push_back(std::move($3));
                    }
     ;

%nterm <std::unique_ptr<Expr::Expr>> argument;
argument:           expr
                    {
                        $$ = std::move($1);
                    }
                |   '@' expr
                    {
                        $$ = std::make_unique<Expr::Splice>(std::move($2));
                    }
     ;


%nterm <std::unique_ptr<Expr::Expr>> expr;
expr:               tINT
                    {
                        $$ = std::make_unique<Expr::Integer>($1);
                    }
                |   tOBJNUM
                    {
                        $$ = std::make_unique<Expr::Objnum>($1);
                    }
                |   tSTR
                    {
                        $$ = std::make_unique<Expr::Str>(std::move($1));
                    }
                |   tREAL
                    {
                        $$ = std::make_unique<Expr::Real>($1);
                    }
                |   '{' arglist '}'
                    {
                        $$ = std::make_unique<Expr::List>(std::move($2));
                    }
                |   tID
                    {
                        Symbol var = parserState.addVar($1);
                        $$ = std::make_unique<Expr::Variable>(var);
                    }
                |   '!' expr
                    {
                        $$ = std::make_unique<Expr::Not>(std::move($2));
                    }
                |   '-' expr   %prec tNEGATE
                    {
                        $$ = std::make_unique<Expr::Negate>(std::move($2));
                    }
                |   tINCREMENT expr
                    {
                        if(!$2->assignable())
                           throw parser::syntax_error("Invalid operand to ++ operator");

                        $$ = std::make_unique<Expr::PreInc>(std::move($2));
                    }
                |   tDECREMENT expr
                    {
                        if(!$2->assignable())
                            throw parser::syntax_error("Invalid operand to -- operator");

                        $$ = std::make_unique<Expr::PreDec>(std::move($2));
                    }
                |   expr tINCREMENT
                    {
                        if(!$1->assignable())
                            throw parser::syntax_error("Invalid operand to ++ operator");

                        $$ = std::make_unique<Expr::PostInc>(std::move($1));
                    }
                |   expr tDECREMENT
                    {
                        if(!$1->assignable())
                            throw parser::syntax_error("Invalid operand to -- operator");

                        $$ = std::make_unique<Expr::PostDec>(std::move($1));

                    }
    ;

%%

namespace Moove { 

#if 0
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
            std::unique_ptr<Target> target(new ScatterTarget(Target::REQUIRED,
                                                                          var->id()));
            targets.push_back(target.get());
            target.release();
        } else if((id = isRestTarget(*item))) {
            std::unique_ptr<Target> target(new ScatterTarget(Target::REST,
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

#endif

    namespace BisonParser {
		void parser::error(const std::string& msg) 
		{
			parserState.error(msg);
		}

		parser::symbol_type yylex(ParserState& parserState)
		{
			return parserState.lexer().nextToken();
		}

	}    //namespace BisonParser

	/**
	 * \brief Type used for communication with parser sub-system
	 *
	 * An instance of this type will both provide a method of communication
	 * with the calling Compiler object as well as hold parsing state
	 * information such as loop depth, variables, and constants.
	 */
	int parseSource(ParserState& parserState)
	{
        BisonParser::parser parser(parserState);
        int ret = parser();
		parserState.parseFinished();
		return ret;
	}

}   //namespace Moove
