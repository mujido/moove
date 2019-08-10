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
   
    virtual Reply doUnaryOp(Opcode op, std::auto_ptr<Variant> operand)const;

    virtual Reply doBinaryOp(Opcode op, std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    //Miscellaneous operators

    virtual Reply doIndexSetOp(std::auto_ptr<Variant> object, std::auto_ptr<Variant> index, std::auto_ptr<Variant> value)const;

    virtual Reply doRangeOp(std::auto_ptr<Variant> object, std::auto_ptr<Variant> startIndex, std::auto_ptr<Variant> endIndex)const;
   
    virtual Reply doRangeSetOp(std::auto_ptr<Variant> object,
                               std::auto_ptr<Variant> startIndex, std::auto_ptr<Variant> endIndex,
                               std::auto_ptr<Variant> value)const;
#if 0
    //Unary operators

    virtual Reply opnot(std::auto_ptr<Variant> operand)const;

    virtual Reply negate(std::auto_ptr<Variant> operand)const;

    virtual Reply length(std::auto_ptr<Variant> operand)const;

    virtual Reply splice(std::auto_ptr<Variant> operand)const;

    //Binary operators

    virtual Reply or(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply and(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply eq(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply ne(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply lt(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply le(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply ge(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply gt(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply in(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply add(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply subtract(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply multiply(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply divide(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply modulo(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply exponent(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

    virtual Reply index(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const;

#endif
};

}   //namespace Moove

#endif    //MOOVE_OPERATORS_HPP

