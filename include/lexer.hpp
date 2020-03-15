//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 *
 * Implementation found in file moove.y.
 */
#ifndef MOOVE_LEXER_HPP
#define MOOVE_LEXER_HPP

#include "mem_pool.hpp"
#include "moove.tab.h"

#include <string>
#include <boost/utility.hpp>
#include <boost/optional/optional.hpp>


namespace Moove {
    class ParserMessages;
    class ParserState;

    class Lexer {
    private:
        using parser = BisonParser::parser;

        ParserState& m_parser;
        std::string                 m_source;
        std::string::const_iterator m_pos;
        std::string::const_iterator m_end;
        unsigned                    m_line;
        bool                        m_enableObjnums;

        static bool isWS(int ch);

        static bool isIDSuffixChar(int ch);

        void skipWS();

        void skipDigits();

        void skipID();

        int peekCur()const
        {
            return (m_pos != m_end) ? *m_pos : -1;
        }

        int peekNext(unsigned idx = 1)const
        {
            return (std::distance(m_pos, m_end) >= idx) ? *(m_pos + idx) : -1;
        }

        bool findEndOfReal();

        boost::optional<parser::symbol_type> tryParseNumber(bool realOK);

        parser::symbol_type parseObjnum();

        parser::symbol_type parseString();

        void parseComment();

        parser::symbol_type parseID();

        parser::symbol_type parseOp();

        Lexer(const Lexer&) = delete;
        Lexer(Lexer&&) = delete;

    public:
        Lexer(ParserState& parser, const std::string& str, ParserMessages& msgs, bool enableObjnums);

        parser::symbol_type nextToken();

        unsigned currentLineNumber() const { return m_line; }
    };

}   //namespace Moove

#endif   //MOOVE_LEXER_HPP
