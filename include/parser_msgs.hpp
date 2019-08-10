//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_PARSER_MSGS_HPP
#define MOOVE_PARSER_MSGS_HPP

#include <string>

namespace Moove {

struct ParserMessages {
   virtual ~ParserMessages()
   {}

   virtual void error(const std::string& msg, unsigned lineNum) = 0;

   virtual void warning(const std::string& msg, unsigned lineNum) = 0;
};

}   //namespace Moove

#endif   //MOOVE_PARSER_MSGS_HPP
