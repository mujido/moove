#include "lexer.hpp"
#include "expr_ast.hpp"
#include "stmt_ast.hpp"
#include "parser_msgs.hpp"
#include "parser_state.hpp"
#include <unordered_map>

using namespace std;

namespace Moove {
    namespace BisonParser {
#include "moove.tab.h"
    }

    using namespace BisonParser;
    
    void Lexer::error(const string& msg)
    {
        m_messages.error(msg, m_line);
    }

    void Lexer::warning(const string& msg)
    {
        m_messages.warning(msg, m_line);
    }

    template<class T>
    inline T* Lexer::addToPool(T* ptr)
    {
        m_parser.addToPool(unique_ptr<ASTPoolObject>(ptr));
        return ptr;
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
                else {
                    error("Malformed floating-point literal");
                    return tINT;
                }
            }

            double value = strtod(string(start, m_pos).c_str(), 0);
            if (errno != ERANGE) {
                m_yylval->real = addToPool(new ASTPoolPtr<double>(new double(value)));
                return tREAL;
            }
            else {
                error("Real value out of range");
                return tINT;
            }
        }
        else {
            long value = strtol(string(start, m_pos).c_str(), 0, 10);
            if (errno != ERANGE && static_cast<int>(value) == value) {
                m_yylval->integer = value;
                return tINT;
            }
            else {
                error("Integer literal out of range");
                return tINT;
            }
        }
    }

    int Lexer::parseObjnum()
    {
        bool negative = false;

        ++m_pos;
        if (peekCur() == '-') {
            negative = true;
            ++m_pos;
        }

        int ret = parseNumber(false) != -1 ? tOBJNUM : -1;

        if (negative && ret == tOBJNUM)
            m_yylval->integer = -m_yylval->integer;

        return ret;
    }

    int Lexer::parseString()
    {
        m_yylval->str = addToPool(new ASTPoolPtr<string>(new string));

        for (string::const_iterator start = ++m_pos; m_pos != m_end; ++m_pos) {
            if (*m_pos == '"') {
                (*m_yylval->str)->append(start, m_pos);
                ++m_pos;
                return tSTR;
            }
            else if (*m_pos == '\\') {
                if (++m_pos != m_end && *m_pos != '\n') {
                    (*m_yylval->str)->append(start, m_pos - 1);
                    **m_yylval->str += *m_pos;
                    start = m_pos + 1;
                }
                else
                    break;
            }
            else if (*m_pos == '\n')
                break;
        }

        error("Missing quote");
        if (m_pos != m_end)
            ++m_pos;

        return tSTR;
    }

    void Lexer::parseComment()
    {
        const char* commentEnd = "*/";

        string::const_iterator pos = search(m_pos + 2, m_end,
            commentEnd, commentEnd + 2);
        if (pos == m_end)
            error("End of program while in a comment");
        else
            m_pos = pos + 2;
    }

    int Lexer::parseID()
    {
        static std::unordered_map<std::string, int> reserved = {
            { "if",		    tIF },
            { "elseif",		tELSEIF },
            { "else",		tELSE },
            { "endif",		tENDIF },
            { "while",		tWHILE },
            { "endwhile",	tENDWHILE },
            { "for",		tFOR },
            { "endfor",		tENDFOR },
            { "fork",		tFORK },
            { "endfork",	tENDFORK },
            { "try",		tTRY },
            { "except",		tEXCEPT },
            { "finally",	tFINALLY },
            { "endtry",		tENDTRY },
            { "switch",		tSWITCH },
            { "case",		tCASE },
            { "default",	tDEFAULT },
            { "endswitch",	tENDSWITCH },
            { "return",		tRETURN },
            { "continue",	tCONTINUE },
            { "break",		tBREAK },
            { "any",		tANY },
            { "in",		    tIN },
        };

        string::const_iterator start = m_pos++;
        skipID();

        unique_ptr<string> id(new string(start, m_pos));

        //Check ID in reserved word set
        auto reservedIter = reserved.find(*id);
        if (reservedIter != reserved.end()) {
            //ID is a reserved word. Return appropriate token
            return reservedIter->second;
        }
        else {
            //ID is not a reserved word
            m_yylval->str = addToPool(new ASTPoolPtr<string>(std::move(id)));
            return tID;
        }
    }

    constexpr unsigned opToInt(char c1, char c2)
    {
        return static_cast<unsigned char>(c1) << 8 | static_cast<unsigned char>(c2);
    };

    int Lexer::parseOp()
    {
        if (m_end - m_pos >= 2) {
            switch (opToInt(m_pos[0], m_pos[1])) {
            case opToInt('+', '='): return tADD_EQ;
            case opToInt('-', '='): return tSUB_EQ;
            case opToInt('*', '='): return tMUL_EQ;
            case opToInt('/', '='): return tDIV_EQ;
            case opToInt('%', '='): return tMOD_EQ;
            case opToInt('^', '='): return tEXP_EQ;
            case opToInt('|', '|'): return tOR;
            case opToInt('&', '&'): return tAND;
            case opToInt('=', '='): return tEQ;
            case opToInt('!', '='): return tNE;
            case opToInt('<', '='): return tLE;
            case opToInt('>', '='): return tGE;
            case opToInt('+', '+'): return tINCREMENT;
            case opToInt('-', '-'): return tDECREMENT;
            case opToInt('=', '>'): return tARROW;
            case opToInt('.', '.'): return tTO;
            default: break;
            }
        }

        //Assume character is a 1-char operator
        return *m_pos++;
    }

    Lexer::Lexer(ParserState& parser, const std::string& str, ParserMessages& msgs, bool enableObjnums) :
        m_parser(parser),
        m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
        m_line(1), m_messages(msgs), m_enableObjnums(enableObjnums),
        m_yylval(nullptr)
    {}

    void Lexer::setYylval(YYSTYPE * yylval)
    {
        m_yylval = yylval;
    }

    int Lexer::nextToken()
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

        return -1;
    }

}