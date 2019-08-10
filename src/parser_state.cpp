//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "parser_state.hpp"

#include "lexer.hpp"
#include "parser_msgs.hpp"

namespace Moove {

ParserState::ParserState(const char* source,
			 ParserMessages& msgs,
			 bool objnums) :
   m_switchDepth(0), m_dollarDepth(0), m_errorFlag(false)
{
   m_lex.reset(new Lexer(source, msgs, objnums));
   m_program.reset(new Program);
}

ParserState::ParserState(const std::string& source, 
			 ParserMessages& msgs, 
			 bool objnums) :
   m_switchDepth(0), m_dollarDepth(0), m_errorFlag(false)
{
   m_lex.reset(new Lexer(source, msgs, objnums));
   m_program.reset(new Program);
}

ParserState::~ParserState()
{}

void ParserState::error(const std::string& msg)
{
   m_errorFlag = true;
   m_lex->error(msg);
}

void ParserState::warning(const std::string& msg)
{
   m_lex->warning(msg);
}

void ParserState::setProgram(std::auto_ptr<Stmt::Block> stmts)
{
   if(!m_errorFlag)
      m_program->setStmts(stmts);
}

void ParserState::addToPool(std::auto_ptr<ASTPoolObject> ptr)
{
   if(ptr.get())
      m_pool.insert(ptr);
}

void ParserState::removeFromPool(const ASTPoolObject* ptr)
{
   if(ptr)
      m_pool.remove(ptr);
}

void ParserState::beginLoop(const std::string* name)
{
   m_blocks.push_back(BlockMarker(BlockMarker::LOOP, 
				  name ? addVar(*name) : Symbol()));
}

void ParserState::endLoop()
{
   if(m_blocks.back().type != BlockMarker::LOOP)
      throw UnmatchedBlock("Moove::ParserState::endLoop() called with previous "
			   "block not a loop");
   m_blocks.pop_back();
}

void ParserState::beginFork()
{
   m_blocks.push_back(BlockMarker(BlockMarker::FORK));
}

void ParserState::endFork()
{
   if(m_blocks.back().type != BlockMarker::FORK)
      throw UnmatchedBlock("Moove::ParserState::endFork() called with previous "
			   "block not a fork");
   m_blocks.pop_back();
}

void ParserState::beginSwitch()
{
   m_blocks.push_back(BlockMarker(BlockMarker::SWITCH));
   ++m_switchDepth;
}

void ParserState::endSwitch()
{
   if(m_blocks.back().type != BlockMarker::SWITCH)
      throw UnmatchedBlock("Moove::ParserState::endSwitch() called with previous"
			   " block not a switch");
   m_blocks.pop_back();
   --m_switchDepth;
}

Symbol ParserState::loopID()const
{
   BlockList::const_reverse_iterator rend = m_blocks.rend();
   for(BlockList::const_reverse_iterator block = m_blocks.rbegin(); 
       block != rend && block->type != BlockMarker::FORK; ++block) 
      if(block->type == BlockMarker::LOOP)
	 return block->id;

   throw UnmatchedBlock("Moov::ParserState::loopID() called without prior loop "
			"block");
}

bool ParserState::checkLoopID(Symbol id)
{
   BlockList::const_reverse_iterator rend = m_blocks.rend();
   for(BlockList::const_reverse_iterator block = m_blocks.rbegin(); 
       block != rend && block->type != BlockMarker::FORK; ++block) 
      if(block->type == BlockMarker::LOOP && (!id || block->id == id))
	 return true;

   return false;
}

Symbol ParserState::addVar(const std::string& name)
{
   return m_program->varTable().insert(name).first;
}

Symbol ParserState::findVar(const std::string& name)const
{
   return m_program->varTable().findSymbol(name);
}

void ParserState::parseFinished()
{
   m_lex.reset();
   m_blocks.clear();
   m_pool.clear(hasErrors());
}

}   //namespace Moove


