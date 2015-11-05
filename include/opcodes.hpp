//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Define the opcodes used for byte compilation and interpretation
 */
#ifndef MOOVE_OPCODES_HPP
#define MOOVE_OPCODES_HPP

#include "codevect.hpp"
#include "immediate.hpp"

namespace Moove {

/**
 * Opcodes handled internally by the VM.
 *
 * \warning
 * Do not forget to change the OpcodeInfo data when modifying the opcode enumerations or adding/removing immediate values.
 */
enum Opcode {
    // ---------- Opcodes handled by the interpreter ----------

    OP_DONE,
    OP_JUMP,                        
    OP_JUMP_FALSE,
    OP_FORK,                
    OP_RETURN,              
    OP_PUSH_LITERAL,       
    OP_PUSH,               
    OP_PUT,                 
    OP_POP,                 
    OP_CALL_BUILTIN,           
    OP_GET_PROP,            
    OP_PUT_PROP,            
    OP_CALL_VERB,
    OP_GET_SYSPROP,
    OP_PUT_SYSPROP,
    OP_CALL_SYSVERB,
    OP_APPEND_LIST,                                             ///< Append an element to a list

    // ---------- Opcodes that must be overloaded for each type with OperatorPackage and OperatorMap ----------

    // ===== Unary Operators =====
    OP_NOT,                                                     ///< Logical NOT
    OP_NEGATE,                                                  ///< Negate

    // ===== Binary Operators =====
    OP_OR,                                                      ///< Logical OR
    OP_AND,                                                     ///< Logical AND
    OP_EQ,                                                      ///< Equality comparison
    OP_NE,                                                      ///< Inequality comparison
    OP_LT,                                                      ///< Less-than comparison
    OP_LE,                                                      ///< Less-than or equality comparison
    OP_GE,                                                      ///< Greater-than or equality comparison
    OP_GT,                                                      ///< Greater-than comparison
    OP_IN,                                                      ///< Element-in-set test
    OP_ADD,                                                     ///< Addition
    OP_SUB,                                                     ///< Subtraction
    OP_MUL,                                                     ///< Multiplication
    OP_DIV,                                                     ///< Division
    OP_MOD,                                                     ///< Modulus
    OP_EXP,                                                     ///< Exponetial

    // ===== Special Operators =====
    OP_INDEX,                                                   ///< List index
    OP_INDEX_SET,                                               ///< Assign an element in a list
    OP_RANGE,                                                   ///< Subrange a list
    OP_RANGE_SET,                                               ///< Modify a list subrange
    OP_LENGTH,                                                  ///< Calculate length of a list without popping from stack
    OP_SPLICE,                                                  ///< List splicing

    NUM_OPCODES                                                 ///< Number of opcodes
};

class OpcodeInfo {
public:
    enum FunctionalGroup {
        FG_NONE         = 0,
        FG_ARITHMETIC   = 1 << 0,
        FG_COMPARISON   = 1 << 1,
        FG_LOGIC        = 1 << 2
    };

    static const unsigned ALL_FUNCTIONAL_GROUPS = FG_ARITHMETIC | FG_COMPARISON | FG_LOGIC;
        
    class Op {
    private:
        Opcode          m_opcode;
        const char*     m_name;
        int             m_immType;

    public:
        Op(Opcode opcode, const char* name, int immType) : m_opcode(opcode), m_name(name), m_immType(immType)
        {}

        Opcode opcode()const
        { return m_opcode; }

        const char* name()const
        { return m_name; }

        bool hasImmediate()const
        { return m_immType != -1; }

        ImmediateValue::Type immediateType()const;

        bool isUnaryOp()const
        { return m_opcode == OP_NOT || m_opcode == OP_NEGATE; }

        bool isBinaryOp()const
        { return m_opcode >= OP_OR && m_opcode <= OP_EXP; }

        FunctionalGroup functionalGroup()const;
    };

private:
    static const Op s_ops[NUM_OPCODES];

public:
    static bool validOp(CodeVector::Byte op)
    { return op < NUM_OPCODES; }

    static const Op& getOp(Opcode op)
    { return s_ops[op]; }
};
   
}    //namespace Moove

#endif   //MOOVE_OPCODES_HPP
   
