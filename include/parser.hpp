//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file parser.hpp
 * \brief Main header file for MOOVE source parsing
 */
#ifndef MOOVE_PARSER_HPP
#define MOOVE_PARSER_HPP

#include "program_ast.hpp"
#include <boost/optional.hpp>

namespace Moove {
    class ParserMessages;

    boost::optional<Program> parse(const std::string& source, ParserMessages& msgs, bool enableObjnums = false);

}   //namespace Moove

#endif    //MOOVE_PARSER_HPP

