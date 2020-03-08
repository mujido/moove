//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_PARSER_STATE_HPP
#define MOOVE_PARSER_STATE_HPP

#include "program_ast.hpp"
#include "stmt_ast.hpp"
#include "symbol_table.hpp"
#include "parser_msgs.hpp"
#include "iterator_lexer_source.hpp"
#include "moove.tab.h"

#include <string>
#include <stdexcept>
#include <vector>
#include <boost/optional.hpp>
#include <boost/utility.hpp>

namespace Moove {

class ParserMessages;

class ParserState {
private:
   struct BlockMarker {
      enum BlockType {
	      LOOP, SWITCH, FORK
      } type;

      Symbol id;

      BlockMarker(BlockType ty, Symbol sym = Symbol()) : type(ty), id(sym)
      {}
   };

   typedef std::vector<BlockMarker> BlockList;

   struct ParserDetails;

   BlockList m_blocks;
   unsigned m_switchDepth;
   unsigned m_dollarDepth;
   bool m_errorFlag;
   ParserMessages& m_msgs;
   std::string m_source;
   IteratorLexerSource<std::string::const_iterator> m_lexerSource;
   std::unique_ptr<ParserDetails> m_details;
   Program m_program;

   ParserState(const ParserState&) = delete;
   ParserState(ParserState&&) = delete;
   ParserState& operator= (const ParserState&) = delete;
   ParserState& operator= (ParserState&&) = delete;

public:
   typedef std::logic_error UnmatchedBlock;

   ParserState(const std::string& source, ParserMessages& msgs, bool objnums);

   ~ParserState();

   unsigned switchDepth()const
   { return m_switchDepth; }

   unsigned dollarDepth()const
   { return m_dollarDepth; }

   bool hasErrors()const
   { return m_errorFlag; }

   const Program& program()const
   { return m_program; }

   Program& program()
   {
       return m_program;
   }
   
   void setProgram(Stmt::Block&& stmts);

   void error(const std::string& msg);

   void warning(const std::string& msg);
      
   void addToPool(std::unique_ptr<ASTPoolObject> ptr);

   void removeFromPool(const ASTPoolObject* ptr);

   void beginLoop(const boost::optional<std::string>& name);

   void endLoop();

   void beginFork();
   
   void endFork();

   void beginSwitch();
   
   void endSwitch();

   Symbol loopID()const;

   bool checkLoopID(Symbol id);

   Symbol addVar(const std::string& name);

   Symbol findVar(const std::string& name)const;

   void incDollarDepth()
   { ++m_dollarDepth; }

   void decDollarDepth()
   { --m_dollarDepth; }

   BisonParser::parser::symbol_type nextToken();

   void parseFinished();
};

}   //namespace Moove

#endif   //MOOVE_PARSER_STATE_HPP


   
