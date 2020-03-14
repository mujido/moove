//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "parser.hpp"
#include "parser_state.hpp"

namespace Moove {

    int parseSource(ParserState& state);

    boost::optional<Program> parse(const std::string& source, ParserMessages& msgs, bool enableObjnums)
    {
        ParserState state(source, msgs, enableObjnums);
        if (0 == parseSource(state))
            return { std::move(state.program()) };
        else
            return boost::none;
    }
}   //namespace Moove
