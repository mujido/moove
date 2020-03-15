//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Implementation of dynamic-type operators on MOOVE variants
 */
#ifndef MOOVE_OPERATORS_HPP
#define MOOVE_OPERATORS_HPP

#include "except.hpp"
#include "opcodes.hpp"
#include "reply.hpp"
#include "variant.hpp"

#include <memory>

namespace Moove {

struct OperatorPackage {
    typedef Exception UndefinedOperator;

    virtual ~OperatorPackage()
    {}
   
    virtual Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> operand)const;

    virtual Reply doBinaryOp(Opcode op, std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    //Miscellaneous operators

    virtual Reply doIndexSetOp(std::unique_ptr<Variant> object, std::unique_ptr<Variant> index, std::unique_ptr<Variant> value)const;

    virtual Reply doRangeOp(std::unique_ptr<Variant> object, std::unique_ptr<Variant> startIndex, std::unique_ptr<Variant> endIndex)const;
   
    virtual Reply doRangeSetOp(std::unique_ptr<Variant> object,
                               std::unique_ptr<Variant> startIndex, std::unique_ptr<Variant> endIndex,
                               std::unique_ptr<Variant> value)const;
#if 0
    //Unary operators

    virtual Reply opnot(std::unique_ptr<Variant> operand)const;

    virtual Reply negate(std::unique_ptr<Variant> operand)const;

    virtual Reply length(std::unique_ptr<Variant> operand)const;

    virtual Reply splice(std::unique_ptr<Variant> operand)const;

    //Binary operators

    virtual Reply or(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply and(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply eq(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply ne(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply lt(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply le(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply ge(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply gt(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply in(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply add(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply subtract(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply multiply(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply divide(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply modulo(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply exponent(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

    virtual Reply index(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const;

#endif
};

}   //namespace Moove

#endif    //MOOVE_OPERATORS_HPP

