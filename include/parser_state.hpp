//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_PARSER_STATE_HPP
#define MOOVE_PARSER_STATE_HPP

#include "ast_base.hpp"
#include "mem_pool.hpp"
#include "program_ast.hpp"
#include "stmt_ast.hpp"
#include "symbol_table.hpp"

#include <string>
#include <stdexcept>
#include <vector>
#include <boost/utility.hpp>

namespace Moove {

class Lexer;
class ParserMessages;

class ParserState : boost::noncopyable {
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

   std::auto_ptr<Lexer>       m_lex;
   MemoryPool<ASTPoolObject>  m_pool;
   BlockList                  m_blocks;
   unsigned                   m_switchDepth;
   unsigned                   m_dollarDepth;
   bool                       m_errorFlag;
   std::auto_ptr<Program>     m_program;

public:
   typedef std::logic_error UnmatchedBlock;

   ParserState(const char* source, ParserMessages& msgs, bool objnums);

   ParserState(const std::string& source, ParserMessages& msgs, bool objnums);

   ~ParserState();

   Lexer& lexer()
   { return *m_lex; }

   unsigned switchDepth()const
   { return m_switchDepth; }

   unsigned dollarDepth()const
   { return m_dollarDepth; }

   bool hasErrors()const
   { return m_errorFlag; }

   const Program& program()const
   { return *m_program; }

   std::auto_ptr<Program> releaseProgram()
   { return m_program; }
   
   void setProgram(std::auto_ptr<Stmt::Block> stmts);

   void error(const std::string& msg);

   void warning(const std::string& msg);
      
   void addToPool(std::auto_ptr<ASTPoolObject> ptr);

   void removeFromPool(const ASTPoolObject* ptr);

   void beginLoop(const std::string* name);

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

   void parseFinished();
};

}   //namespace Moove

#endif   //MOOVE_PARSER_STATE_HPP


   
