//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "parser_state.hpp"
#include "parser_msgs.hpp"
#include "moove_lexer.l.h"

namespace Moove {

    struct ParserState::ParserDetails
    {
        yyscan_t m_scanner;

        ParserDetails(ParserState* pstate)
        {
            Moove_lex_init_extra(&pstate->m_lexerSource, &m_scanner);
        }

        ~ParserDetails()
        {
            Moove_lex_destroy(m_scanner);
        }
    };

    ParserState::ParserState(const std::string& source, ParserMessages& msgs, bool objnums) :
        m_switchDepth(0), m_dollarDepth(0), m_errorFlag(false),
        m_msgs(msgs), 
        m_source(source),
        m_lexerSource(m_source.cbegin(), m_source.cend()),
        m_details(std::make_unique<ParserDetails>(this))
    {
    }

    ParserState::~ParserState() = default;

    void ParserState::error(const std::string& msg)
    {
        m_errorFlag = true;
        m_msgs.error(msg, Moove_get_lineno(m_details->m_scanner));
    }

    void ParserState::warning(const std::string& msg)
    {
        m_msgs.warning(msg, Moove_get_lineno(m_details->m_scanner));
    }

    void ParserState::setProgram(Stmt::Block&& stmts)
    {
        if (!m_errorFlag)
            m_program.setStmts(std::move(stmts));
    }

    void ParserState::beginLoop(const boost::optional<std::string>& name)
    {
        m_blocks.push_back(BlockMarker(BlockMarker::LOOP,
            name ? addVar(*name) : Symbol()));
    }

    void ParserState::endLoop()
    {
        if (m_blocks.back().type != BlockMarker::LOOP)
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
        if (m_blocks.back().type != BlockMarker::FORK)
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
        if (m_blocks.back().type != BlockMarker::SWITCH)
            throw UnmatchedBlock("Moove::ParserState::endSwitch() called with previous"
                " block not a switch");
        m_blocks.pop_back();
        --m_switchDepth;
    }

    Symbol ParserState::loopID()const
    {
        BlockList::const_reverse_iterator rend = m_blocks.rend();
        for (BlockList::const_reverse_iterator block = m_blocks.rbegin();
            block != rend && block->type != BlockMarker::FORK; ++block)
            if (block->type == BlockMarker::LOOP)
                return block->id;

        throw UnmatchedBlock("Moov::ParserState::loopID() called without prior loop "
            "block");
    }

    bool ParserState::checkLoopID(Symbol id)
    {
        BlockList::const_reverse_iterator rend = m_blocks.rend();
        for (BlockList::const_reverse_iterator block = m_blocks.rbegin();
            block != rend && block->type != BlockMarker::FORK; ++block)
            if (block->type == BlockMarker::LOOP && (!id || block->id == id))
                return true;

        return false;
    }

    Symbol ParserState::addVar(const std::string& name)
    {
        return m_program.varTable().insert(name).first;
    }

    Symbol ParserState::findVar(const std::string& name)const
    {
        return m_program.varTable().findSymbol(name);
    }

    BisonParser::parser::symbol_type ParserState::nextToken()
    {
        return Moove_lex(m_details->m_scanner);
    }

    void ParserState::parseFinished()
    {
        m_blocks.clear();
    }

}   //namespace Moove


