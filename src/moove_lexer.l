%option noyywrap nounput noinput yylineno reentrant batch never-interactive 
%option prefix="Moove_"
%option extra-type="::Moove::ILexerSource*"

%top {
#include "ilexer_source.hpp"
#include "moove.tab.h"

#define YY_DECL Moove::BisonParser::parser::symbol_type Moove_lex(yyscan_t yyscanner)

}

%{
#include "lexer_helpers.hpp"
#include <boost/format.hpp>

using boost::format;

using namespace Moove;
using namespace Moove::BisonParser;

#define YY_INPUT(buf, result, max_size) \
    { \
        auto readSize = yyextra->read(buf, static_cast<std::size_t>(max_size)); \
        result = readSize > 0 ? static_cast<int>(readSize) : YY_NULL; \
    }

%}

dseq        [[:digit:]]+
dseq_opt    [[:digit:]]*    
frac        ({dseq_opt}"."{dseq})|{dseq}"."
exp         [eE][+-]?{dseq}
exp_opt     {exp}?
dfc         {frac}{exp_opt}

id          [[:alpha:]_][[:alnum:]_]*
str         \"([^\"\\\n]|(\\[^\n]))*\"

%{
/* "if"            parser::make_tIF();
"elseif"        parser::make_tELSEIF();
"else"          parser::make_tELSE();
"endif"         parser::make_tENDIF();
"while"         parser::make_tWHILE();
"endwhile"      parser::make_tENDWHILE();
"for"           parser::make_tFOR();
"endfor"        parser::make_tENDFOR();
"fork"          parser::make_tFORK();
"endfork"       parser::make_tENDFORK();
"try"           parser::make_tTRY();
"except"        parser::make_tEXCEPT();
"finally"       parser::make_tFINALLY();
"endtry"        parser::make_tENDTRY();
"switch"        parser::make_tSWITCH();
"case"          parser::make_tCASE();
"default"       parser::make_tDEFAULT();
"endswitch"     parser::make_tENDSWITCH();
"return"        parser::make_tRETURN();
"continue"      parser::make_tCONTINUE();
"break"         parser::make_tBREAK();
"any"           parser::make_tANY();
"in"            parser::make_tIN(); */
%}

%%


"+="            return parser::make_tADD_EQ();
"-="            return parser::make_tSUB_EQ();
"*="            return parser::make_tMUL_EQ();
"/="            return parser::make_tDIV_EQ();
"%="            return parser::make_tMOD_EQ();
"^="            return parser::make_tEXP_EQ();
"||"            return parser::make_tOR();
"&&"            return parser::make_tAND();
"=="            return parser::make_tEQ();
"!="            return parser::make_tNE();
"<="            return parser::make_tLE();
">="            return parser::make_tGE();
"++"            return parser::make_tINCREMENT();
"--"            return parser::make_tDECREMENT();
"=>"            return parser::make_tARROW();
".."            return parser::make_tTO();

{dseq}          return parseInteger(yytext);

{dfc}           return parseReal(yytext); 

"#"{dseq}       return parseObjnum(yytext); 

{id}            return parseID(yytext);

{str}           return parseStr(yytext);

[-=?|`'<>+*/%^!,.:[\]$;{}]     return parser::symbol_type(*yytext);    // single character operators

[[:space:]]     /* ignore spaces */

<<EOF>>         return parser::make_END();

.               throw parser::syntax_error(str(format("Invalid character: '%c'") % yytext[0]));


%%
