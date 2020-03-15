#pragma once

#include <cstddef>

namespace Moove
{

    struct ILexerSource
    {
        virtual std::size_t read(char* buf, std::size_t maxSize) = 0;

        virtual ~ILexerSource() = default;
    };

}