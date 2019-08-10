#include "msg_handler.hpp"

#include <iostream>

void MessageHandler::error(const std::string& msg, unsigned lineNum)
{
   std::cerr << "Error, line " << lineNum + m_lineOffset << ": " << msg << std::endl;
}
   
void MessageHandler::warning(const std::string& msg, unsigned lineNum)
{
   std::cerr << "Warning, line " << lineNum + m_lineOffset << ": " << msg << std::endl;
}

