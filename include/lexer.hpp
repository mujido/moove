//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 *
 * Implementation found in file moove.y.
 */
#ifndef MOOVE_LEXER_HPP
#define MOOVE_LEXER_HPP

#include "mem_pool.hpp"

#include <string>
#include <boost/utility.hpp>


namespace Moove {
    namespace BisonParser {
        union YYSTYPE;
    }

    class ParserMessages;
    class ParserState;

    class Lexer {
    private:
        ParserState& m_parser;
        std::string                 m_source;
        std::string::const_iterator m_pos;
        std::string::const_iterator m_end;
        unsigned                    m_line;
        ParserMessages& m_messages;
        bool                        m_enableObjnums;
        BisonParser::YYSTYPE* m_yylval;

        static bool isWS(char ch);

        static bool myIsDigit(char ch);

        static bool isIDSuffixChar(char ch);

        template<class T>
        inline T* addToPool(T* ptr);

        void skipWS();

        void skipDigits();

        void skipID();

        int peekCur()const
        {
            return (m_pos != m_end) ? *m_pos : -1;
        }

        int peekNext()const
        {
            return (m_pos + 1 != m_end) ? *(m_pos + 1) : -1;
        }

        int parseNumber(bool realOK);

        int parseObjnum();

        int parseString();

        void parseComment();

        int parseID();

        int parseOp();

        Lexer(const Lexer&) = delete;
        Lexer(Lexer&&) = delete;

    public:
        Lexer(ParserState& parser, const std::string& str, ParserMessages& msgs, bool enableObjnums);

        bool isYylvalSet() { return m_yylval != nullptr; }

        void setYylval(BisonParser::YYSTYPE* yylval);

        int nextToken();

        void error(const std::string& msg);

        void warning(const std::string& msg);
    };

}   //namespace Moove

#endif   //MOOVE_LEXER_HPP
