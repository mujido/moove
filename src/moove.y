%require "3.4"
%language "c++"
%defines
%define api.namespace {Moove::BisonParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%skeleton "lalr1.cc"
%parse-param { ParserState& parserState }
%lex-param { ParserState& parserState }

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
stmt:               if_stmt { $$ = std::move($1); }
                |   expr_stmt { $$ = std::move($1); }
    ;


%nterm <std::unique_ptr<Stmt::Stmt>> if_stmt;
if_stmt:            tIF '(' expr ')' stmts elseifs else_block tENDIF
                    {
                        if($7)
                            $6.push_back(std::move(*$7));

                        $$ = std::make_unique<Stmt::If>(std::move($3), std::move($5), std::move($6));
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

%nterm <std::unique_ptr<Stmt::Stmt>> expr_stmt;
expr_stmt:          expr ';'
                    {
                        $$ = std::make_unique<Stmt::ExprStmt>(std::move($1));
                    }
    ;

%nterm <std::unique_ptr<Expr::Expr>> numeric_expr;
numeric_expr:       tINT
                    {
                        $$ = std::make_unique<Expr::Integer>($1);
                    }
                
                |   '-' tINT %prec tNEGATE
                    {
                        $$ = std::make_unique<Expr::Integer>(-$2);
                    }
                |   tREAL
                    {
                        $$ = std::make_unique<Expr::Real>($1);
                    }
                |   '-' tREAL %prec tNEGATE
                    {
                        $$ = std::make_unique<Expr::Real>(-$2);
                    }
    ;

%nterm <std::unique_ptr<Expr::Expr>> expr;
expr:               numeric_expr
                    {
                        $$ = std::move($1);
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
