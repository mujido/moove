#ifndef MOOSH_MSG_HANDLER_HPP
#define MOOSH_MSG_HANDLER_HPP

#include <parser_msgs.hpp>

class MessageHandler : public Moove::ParserMessages {
private:
   unsigned m_lineOffset;

public:
   MessageHandler(unsigned lineOffset) : m_lineOffset(lineOffset)
   {}

   void error(const std::string& msg, unsigned lineNum);
   
   void warning(const std::string& msg, unsigned lineNum);
};

#endif
