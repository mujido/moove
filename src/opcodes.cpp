#include "opcodes.hpp"
#include "except.hpp"

namespace Moove {

const OpcodeInfo::Op OpcodeInfo::s_ops[LAST_OPCODE + 1] = {
    OpcodeInfo::Op(OP_DONE,             "DONE",                 -1),
    OpcodeInfo::Op(OP_JUMP,             "JUMP",                 ImmediateValue::LABEL),
    OpcodeInfo::Op(OP_JUMP_FALSE,       "JUMP_FALSE",           ImmediateValue::LABEL),
    OpcodeInfo::Op(OP_FORK,             "FORK",                 ImmediateValue::FORK),
    OpcodeInfo::Op(OP_RETURN,           "RETURN",               -1),
    OpcodeInfo::Op(OP_PUSH_LITERAL,     "PUSH_LITERAL",         ImmediateValue::LITERAL),
    OpcodeInfo::Op(OP_PUSH,             "PUSH",                 ImmediateValue::TEMP),
    OpcodeInfo::Op(OP_PUT,              "PUT",                  ImmediateValue::TEMP),
    OpcodeInfo::Op(OP_POP,              "POP",                  -1),
    OpcodeInfo::Op(OP_CALL_BUILTIN,     "CALL_BUILTIN",         -1),
    OpcodeInfo::Op(OP_GET_PROP,         "GET_PROP",             -1),
    OpcodeInfo::Op(OP_PUT_PROP,         "PUT_PROP",             -1),
    OpcodeInfo::Op(OP_CALL_VERB,        "CALL_VERB",            -1),
    OpcodeInfo::Op(OP_GET_SYSPROP,      "GET_SYSPROP",          -1),
    OpcodeInfo::Op(OP_PUT_SYSPROP,      "PUT_SYSPROP",          -1),
    OpcodeInfo::Op(OP_CALL_SYSVERB,     "CALL_SYSVERB",         -1),
    OpcodeInfo::Op(OP_APPEND_LIST,      "APPEND_LIST",          -1),

    OpcodeInfo::Op(OP_NOT,              "NOT",                  -1),
    OpcodeInfo::Op(OP_NEGATE,           "NEGATE",               -1),

    OpcodeInfo::Op(OP_OR,               "OR",                   -1),
    OpcodeInfo::Op(OP_AND,              "AND",                  -1),
    OpcodeInfo::Op(OP_EQ,               "EQ",                   -1),
    OpcodeInfo::Op(OP_NE,               "NE",                   -1),
    OpcodeInfo::Op(OP_LT,               "LT",                   -1),
    OpcodeInfo::Op(OP_LE,               "LE",                   -1),
    OpcodeInfo::Op(OP_GE,               "GE",                   -1),
    OpcodeInfo::Op(OP_GT,               "GT",                   -1),
    OpcodeInfo::Op(OP_IN,               "IN",                   -1),
    OpcodeInfo::Op(OP_ADD,              "ADD",                  -1),
    OpcodeInfo::Op(OP_SUB,              "SUB",                  -1),
    OpcodeInfo::Op(OP_MUL,              "MUL",                  -1),
    OpcodeInfo::Op(OP_DIV,              "DIV",                  -1),
    OpcodeInfo::Op(OP_MOD,              "MOD",                  -1),
    OpcodeInfo::Op(OP_EXP,              "EXP",                  -1),

    OpcodeInfo::Op(OP_INDEX,            "INDEX",                -1),
    OpcodeInfo::Op(OP_INDEX_SET,        "INDEX_SET",            -1),
    OpcodeInfo::Op(OP_RANGE,            "RANGE",                -1),
    OpcodeInfo::Op(OP_RANGE_SET,        "RANGE_SET",            -1),
    OpcodeInfo::Op(OP_LENGTH,           "LENGTH",               -1),
    OpcodeInfo::Op(OP_SPLICE,           "SPLICE",               -1)
};

ImmediateValue::Type OpcodeInfo::Op::immediateType()const
{
    MOOVE_ASSERT(m_immType != -1, "invalid immediate value type");
    return static_cast<ImmediateValue::Type>(m_immType);
}

OpcodeInfo::FunctionalGroup OpcodeInfo::Op::functionalGroup()const
{
    switch(m_opcode) {
        case OP_NEGATE:
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
        case OP_EXP:
            return FG_ARITHMETIC;

        case OP_EQ:
        case OP_NE:
        case OP_LT:
        case OP_LE:
        case OP_GE:
        case OP_GT:
            return FG_COMPARISON;

        case OP_NOT:
        case OP_OR:
        case OP_AND:
            return FG_LOGIC;

        default:
            break;
    }

    return FG_NONE;
}
    
}       //namespace Moove
