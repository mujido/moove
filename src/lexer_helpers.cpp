#include "lexer_helpers.hpp"
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <type_traits>
#include <unordered_map>

namespace Moove 
{
    using namespace BisonParser;

    bool isValidIntValue(long lval)
    {
        if constexpr (sizeof(long) == sizeof(int))
            return true;
        else
            return lval >= std::numeric_limits<int>::min() || lval <= std::numeric_limits<int>::max();
    }

    symbol_type parseInteger(const char* text)
    {
        long value = std::strtol(text, nullptr, 10);
        if (errno == ERANGE || !isValidIntValue(value)) 
            throw parser::syntax_error("integer value is out of range");

        return parser::make_tINT(static_cast<int>(value));
    }

    symbol_type parseReal(const char* text)
    {
        double value = std::strtod(text, nullptr);
        if (value == HUGE_VAL)
            throw parser::syntax_error("real value is out of range");

        return parser::make_tREAL(value);
    }

    symbol_type parseObjnum(const char* text)
    {
        long value = std::strtol(text + 1, nullptr, 10);
        if (errno == ERANGE || !isValidIntValue(value))
            throw parser::syntax_error("objnum value is out of range");

        return parser::make_tOBJNUM(static_cast<int>(value));   
    }

    symbol_type parseID(const char* text)
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

        std::string id(text);

        //Check ID in reserved word set
        auto reservedIter = reserved.find(id);
        if (reservedIter != reserved.end()) {
            //ID is a reserved word. Return appropriate token
            return symbol_type(reservedIter->second);
        }
        else {
            //ID is not a reserved word
            return parser::make_tID(std::move(id));
        }
    }

    symbol_type parseStr(const char* text)
    {
        std::string str;

        for (auto pos = text + 1; *pos != '"' && *pos != '\0';)
        {
            auto escape = std::strchr(pos, '\\');
            if (escape == nullptr)
            {
                str.append(pos, std::strlen(pos) - 1);
                break;
            }

            str.append(pos, escape);
            str.push_back(escape[1]);
            pos = escape + 2;
        }

        return parser::make_tSTR(std::move(str));
    }
}
