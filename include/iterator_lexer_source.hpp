#pragma once

#include "ilexer_source.hpp"
#include <iterator>

namespace Moove
{
    template<typename Iterator>
    class IteratorLexerSource : public ILexerSource
    {
    public:
        IteratorLexerSource(Iterator first, Iterator last) :
            m_pos(first), m_end(last)
        {}

        virtual std::size_t read(char* buf, std::size_t maxSize) override;

    private:
        Iterator m_pos;
        Iterator m_end;
    };

    template<typename Iterator>
    std::size_t IteratorLexerSource<Iterator>::read(char* buf, std::size_t maxSize)
    {
        auto availBytes = static_cast<std::size_t>(std::distance(m_pos, m_end));
        auto readSize = std::min(availBytes, maxSize);
        auto readStart = m_pos;
        std::advance(m_pos, readSize);
        std::copy(readStart, m_pos, buf);
        return readSize;
    }
}