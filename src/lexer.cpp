#include "lexer.hpp"
#include "expr_ast.hpp"
#include "stmt_ast.hpp"
#include "parser_msgs.hpp"
#include "parser_state.hpp"
#include "moove.tab.h"
#include <unordered_map>
#include <boost/format.hpp>

using namespace std;
using boost::format;

namespace Moove {

    using namespace BisonParser;
    
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

    parser::symbol_type Lexer::parseNumber(bool realOK)
    {
        string::const_iterator start = m_pos;

        //integer portion
        skipDigits();

        char ch = peekCur();
        if (realOK &&
            ((ch == '.' && peekNext() != '.') || ch == 'e' || ch == 'E')) {
            //fractional portion
            if (ch == '.') {
                ++m_pos;
                skipDigits();
                ch = peekCur();
            }

            //exponent portion
            if (ch == 'e' || ch == 'E') {
                ++m_pos;
                ch = peekCur();
                if (ch == '-' || ch == '+')
                    ++m_pos;

                if (isdigit(peekCur()))
                    skipDigits();
                else 
                    throw parser::syntax_error("Malformed floating-point literal");
            }

            double value = strtod(string(start, m_pos).c_str(), 0);
            if (errno == ERANGE)
                throw parser::syntax_error("Real value out of range");

            return parser::make_tREAL(value);
        } else {
            try 
            {
                int value = std::stoi(string(start, m_pos), 0, 10);
                return parser::make_tINT(value);
            }
            catch (std::invalid_argument&) 
            {
                throw parser::syntax_error("invalid integer literal");
            }
            catch (std::range_error&)
            {
                throw parser::syntax_error("Intger value out of range");
            }
        }
    }

    parser::symbol_type Lexer::parseObjnum()
    {
        if (!m_enableObjnums)
            throw parser::syntax_error(str(format("invalid character '%c'") % *m_pos));

        bool negative = false;

        ++m_pos;
        if (peekCur() == '-') {
            negative = true;
            ++m_pos;
        }

        auto number = parseNumber(false);
        int value = number.value.as<int>();
        if (negative)
            value = -value;

        return parser::make_tOBJNUM(value);
    }

    parser::symbol_type Lexer::parseString()
    {
        std::string str;

        for (auto start = ++m_pos; m_pos != m_end; ++m_pos) 
        {
            if (*m_pos == '"') 
            {
                str.append(start, m_pos);
                break;
            }
            else if (*m_pos == '\\') 
            {
                if (++m_pos != m_end && *m_pos != '\n')
                {
                    str.append(start, m_pos - 1);
                    str.insert(str.end(), *m_pos);
                    start = m_pos + 1;
                }
                else
                    throw parser::syntax_error("Missing quote");
            }
            else if (*m_pos == '\n')
                throw parser::syntax_error("Missing quote");
        }

        return parser::make_tSTR(std::move(str));
    }

    void Lexer::parseComment()
    {
        const char* commentEnd = "*/";

        string::const_iterator pos = search(m_pos + 2, m_end, commentEnd, commentEnd + 2);
        if (pos == m_end)
            throw parser::syntax_error("End of program while in a comment");
        else
            m_pos = pos + 2;
    }

    parser::symbol_type Lexer::parseID()
    {
        using token = parser::token;

        static std::unordered_map<std::string, int> reserved = {
            { "if",		    token::tIF },
            { "elseif",		token::tELSEIF },
            { "else",		token::tELSE },
            { "endif",		token::tENDIF },
            { "while",		token::tWHILE },
            { "endwhile",	token::tENDWHILE },
            { "for",		token::tFOR },
            { "endfor",		token::tENDFOR },
            { "fork",		token::tFORK },
            { "endfork",	token::tENDFORK },
            { "try",		token::tTRY },
            { "except",		token::tEXCEPT },
            { "finally",	token::tFINALLY },
            { "endtry",		token::tENDTRY },
            { "switch",		token::tSWITCH },
            { "case",		token::tCASE },
            { "default",	token::tDEFAULT },
            { "endswitch",	token::tENDSWITCH },
            { "return",		token::tRETURN },
            { "continue",	token::tCONTINUE },
            { "break",		token::tBREAK },
            { "any",		token::tANY },
            { "in",		    token::tIN },
        };

        string::const_iterator start = m_pos++;
        skipID();

        string id(start, m_pos);

        //Check ID in reserved word set
        auto reservedIter = reserved.find(id);
        if (reservedIter != reserved.end()) {
            //ID is a reserved word. Return appropriate token
            return parser::symbol_type(reservedIter->second);
        }
        else {
            //ID is not a reserved word
            return parser::make_tID(std::move(id));
        }
    }

    constexpr unsigned opToInt(char c1, char c2)
    {
        return static_cast<unsigned char>(c1) << 8 | static_cast<unsigned char>(c2);
    };

    parser::symbol_type Lexer::parseOp()
    {
        using token = parser::token;
        using symbol_type = parser::symbol_type;

        char ch = *m_pos++;

        if (m_end - m_pos >= 1) {
            auto possOp = opToInt(ch, *m_pos++);

            switch (possOp) {
                case opToInt('+', '='): return symbol_type(token::tADD_EQ);
                case opToInt('-', '='): return symbol_type(token::tSUB_EQ);
                case opToInt('*', '='): return symbol_type(token::tMUL_EQ);
                case opToInt('/', '='): return symbol_type(token::tDIV_EQ);
                case opToInt('%', '='): return symbol_type(token::tMOD_EQ);
                case opToInt('^', '='): return symbol_type(token::tEXP_EQ);
                case opToInt('|', '|'): return symbol_type(token::tOR);
                case opToInt('&', '&'): return symbol_type(token::tAND);
                case opToInt('=', '='): return symbol_type(token::tEQ);
                case opToInt('!', '='): return symbol_type(token::tNE);
                case opToInt('<', '='): return symbol_type(token::tLE);
                case opToInt('>', '='): return symbol_type(token::tGE);
                case opToInt('+', '+'): return symbol_type(token::tINCREMENT);
                case opToInt('-', '-'): return symbol_type(token::tDECREMENT);
                case opToInt('=', '>'): return symbol_type(token::tARROW);
                case opToInt('.', '.'): return symbol_type(token::tTO);
                default: break;
            }

            // Did not match multi-char ops so rewind m_pos
            --m_pos;
        }
         
        static const char singleCharOps[] = "=?|`'<>+-*/%^!.:[]$;";
        auto scOpsEnd = singleCharOps + sizeof(singleCharOps) - 1;
        auto scOpPos = std::find(singleCharOps, scOpsEnd, ch);
        if (scOpPos == scOpsEnd)
            throw parser::syntax_error(str(format("invalid character '%c'") % ch));

        return parser::symbol_type(ch);
    }

    Lexer::Lexer(ParserState& parser, const std::string& str, ParserMessages& msgs, bool enableObjnums) :
        m_parser(parser),
        m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
        m_line(1), m_enableObjnums(enableObjnums)
    {}

    parser::symbol_type Lexer::nextToken()
    {
        skipWS();

        while (m_pos != m_end) {
            char ch = *m_pos;

            if (ch == '\n') {
                ++m_line;
                ++m_pos;
                skipWS();
            }
            else if (ch == '/' && peekNext() == '*') {
                parseComment();
            }
            else if (ch == '"') {
                return parseString();
            }
            else if (isdigit(ch) || (ch == '.' && isdigit(peekNext()))) {
                return parseNumber(true);
            }
            else if (m_enableObjnums && ch == '#') {
                return parseObjnum();
            }
            else if (isalpha(ch) || ch == '_') {
                return parseID();
            }
            else
                return parseOp();
        }

        return parser::make_END();
    }

}