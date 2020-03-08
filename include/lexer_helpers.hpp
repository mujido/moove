#pragma once

#include "moove.tab.h"

namespace Moove 
{
    using symbol_type = BisonParser::parser::symbol_type;

    symbol_type parseInteger(const char* text);
    symbol_type parseReal(const char* text);
    symbol_type parseObjnum(const char* text);
    symbol_type parseID(const char* text);
    symbol_type parseStr(const char* text);
}
