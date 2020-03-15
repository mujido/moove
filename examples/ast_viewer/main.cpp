//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "parser.hpp"
#include "parser_msgs.hpp"

#include "graphviz_visitor.hpp"

#include <iostream>
#include <string>

using namespace Moove;

struct MyMessages : public ParserMessages {
   void error(const std::string& msg, unsigned lineNum)
   {
      std::cerr << "Error, line " << lineNum << ": " << msg << std::endl;
   }

   void warning(const std::string& msg, unsigned lineNum)
   {
      std::cerr << "Warning, line " << lineNum << ": " << msg << std::endl;
   }
};

int main()
{
   try {
      std::string source, line;

      while(std::getline(std::cin, line))
	 source += line + '\n';

      MyMessages msgs;
      Parser parser;
      if(parser.parse(source, msgs, true)) {
	 std::unique_ptr<Program> program = parser.releaseProgram();
	 GraphvizVisitor graphVisitor(std::cout);

	 graphVisitor.visit(*program);
      } else {
	 return 1;
      }
   } catch(const std::exception& e) {
      std::cerr << "Exception caught: " << e.what() << std::endl;
      return 2;
   }

   return 0;
}
