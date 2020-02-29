//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "parser.hpp"

#include "parser_state.hpp"

namespace Moove {

    int parseSource(ParserState& state);

    Parser::Parser(const char* source, ParserMessages& msgs, bool objnums)
    {
        parse(source, msgs, objnums);
    }

    Parser::Parser(const std::string& source, ParserMessages& msgs, bool objnums)
    {
        parse(source, msgs, objnums);
    }

    bool Parser::parse(const char* source, ParserMessages& msgs, bool objnums)
    {
        m_state.reset(new ParserState(source, msgs, objnums));
        parseSource(*m_state);
        return !hasErrors();
    }

    bool Parser::parse(const std::string& source, ParserMessages& msgs, bool objnums)
    {
        m_state.reset(new ParserState(source, msgs, objnums));
        parseSource(*m_state);
        return !hasErrors();
    }

    bool Parser::hasErrors()const
    {
        return m_state->hasErrors();
    }

    std::unique_ptr<Program> Parser::releaseProgram()
    {
        if (hasErrors())
            throw MisformedProgram("A misformed program was illegally attempted to be converted to a Moove::Program in Moove::Parser::releaseProgram()");

        return std::make_unique<Program>(std::move(m_state->program()));
    }

}   //namespace Moove
