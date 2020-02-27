/**
 * \file
 */
#ifndef MOOVE_DEFAULT_OPS_HPP
#define MOOVE_DEFAULT_OPS_HPP

#include "except.hpp"
#include "op_package.hpp"
#include "op_wrappers.hpp"
#include "opcodes.hpp"
#include "reply.hpp"
#include "variant.hpp"
#include "intvar.hpp"
#include "realvar.hpp"
#include "strvar.hpp"

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Moove {

/**
 * Base class to the various operator sets.
 *
 * Defines some methods that are used by all the OperatorSet descendents.
 */
template<class ResultVariant>
struct OperatorDispatchBase {
    template<class T>
    static const T& variant_cast(const Variant& var)
    {
#ifdef NDEBUG
        return *static_cast<const T*>(&var);
#else
        return dynamic_cast<const T&>(var);
#endif
    }

    template<class T>
    static T& variant_cast(Variant& var)
    {
#ifdef NDEBUG
        return *static_cast<T*>(&var);
#else
        return dynamic_cast<T&>(var);
#endif
    }

    /**
     * Create a shared pointer to a new instance of ResultVariant.
     *
     * @return Shared pointer to a new ResultVariant instance.
     */
    static boost::shared_ptr<ResultVariant> createResult()
    { return boost::shared_ptr<ResultVariant>(&variant_cast<ResultVariant>(*ResultVariant::classFactory().create())); }
};

template<class ResultVariant, class OperandVariant>
struct UnaryOperatorDispatch  : public OperatorDispatchBase<ResultVariant> {
    static const OperandVariant& castOperand(const Variant& var)
    { return OperatorDispatchBase<ResultVariant>::variant_cast<OperandVariant>(var); }
};

template<class ResultVariant, class LeftVariant, class RightVariant>
struct BinaryOperatorDispatch : public OperatorDispatchBase<ResultVariant> {
    /**
     * Cast a variant to the LeftVariant type.
     *
     * @param var A variant instance to attempt to cast.
     * @return A reference to a LeftVariant type interface to var.
     * @throws std::bad_cast if var cannot be casted to LeftVariant.
     */
    static const LeftVariant& castLeft(const Variant& var)
    { return OperatorDispatchBase<ResultVariant>::variant_cast<LeftVariant>(var); }

    /**
     * Cast a variant to the RightVariant type.
     *
     * @param var A variant instance to attempt to cast.
     * @return A reference to a RightVariant type interface to var.
     * @throws std::bad_cast if var cannot be casted to RightVariant.
     */
    static const RightVariant& castRight(const Variant& var)
    { return OperatorDispatchBase<ResultVariant>::variant_cast<RightVariant>(var); }
};


/**
 * Wraps arithmetic operations with Variants using standard C++ operator on internal values.
 *
 * Each arithmetic opcode is wrapped with its C++ equivalent. Uses Moove::get_variant_value<> and Moove::set_variant_value<> 
 * to retrieve values from the Variants that can be used with C++ operators. There are certain requirements for using this 
 * template:
 *      - LeftVariant and RightVariant must define a .value() method or specialize on Moove::get_variant_value<>.
 *      - ResultVariant must define a .setValue() or specialize on Moove::set_variant_value<>.
 *      - The result of the call to the appropriate operator wrapper (see OperatorWrappers) must result in a valid value. 
 *        The default action of the wrapper is to call the C++ operator with the <tt>get_variant_value(variant)</tt> value
 *        of both leftVar and rightVar. The wrappers can be specialized to produce different results, however.
 *      - ResultVariant must define a <tt>create()</tt> method that accepts the result of the operator wrapper call as an
 *        argument.
 *
 * Also, note that unary operators are only implemented for instances of the template where LeftVariant is the same type
 * as RightVariant. In the case where they differ, an exception is thrown.
 *
 * The opcodes are defined in the following ways:
 *      <table>
 *              <tr>
 *                      <th>Opcode</th>
 *                      <th>C++ expression</th>
 *              </tr>
 *              <tr>
 *                      <td>OP_NEGATE</td>
 *                      <td>variant_negate(var)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_ADD</th>
 *                      <td>variant_add(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_SUB</th>
 *                      <td>variant_sub(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_MUL</th>
 *                      <td>variant_mul(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_DIV</th>
 *                      <td>variant_div(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_MOD</th>
 *                      <td>variant_mod(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_EXP</th>
 *                      <td>variant_exp(leftVar, rightVar)</td>
 *              </tr>
 *      </table>
 */
template<class ResultVariant, class OperandVar>
struct UnaryArithmeticOperatorDispatch : public UnaryOperatorDispatch<ResultVariant, OperandVar> {
    using UnaryOperatorDispatch<ResultVariant, OperandVariant>::createResult;
    using UnaryOperatorDispatch<ResultVariant, OperandVariant>::castOperand;

    typedef typename ResultVariant::value_type result_value_type;

    // called if LeftVariant and RightVariant are the same type
    static Reply dispatch(Opcode op, std::unique_ptr<Variant> var)
    {
        using namespace OperatorWrappers;

        boost::shared_ptr<ResultVariant> resultVar(UnaryOperatorDispatch<ResultVariant, OperandVariant>::createResult());
        
        if (op == OP_NEGATE)
            set_variant_value(*resultVar, variant_negate<result_value_type>(castOperand(*var)));
        else
            MOOVE_THROW("unexpected opcode");
        
        return Reply(Reply::NORMAL, resultVar);
    }
};

template<class ResultVariant, class LeftVariant, class RightVariant = LeftVariant>
struct BinaryArithmeticOperatorDispatch : public BinaryOperatorDispatch<ResultVariant, LeftVariant, RightVariant> {
    using OperatorSetBase<ResultVariant, OperandVariant>::castLeft;
    using OperatorSetBase<ResultVariant, OperandVariant>::castRight;
    using OperatorSetBase<ResultVariant, OperandVariant>::createResult;

    typedef typename ResultVariant::value_type result_value_type;

    static Reply dispatch(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)
    {
        using namespace OperatorWrappers;

        boost::shared_ptr<ResultVariant> resultVar(createResult());

	switch(op) {
	    case OP_ADD:
                set_variant_value(*resultVar, variant_add<result_value_type>(castLeft(*leftVar), 
                                                                             castRight(*rightVar)));
		break;

	    case OP_SUB:
		set_variant_value(*resultVar, variant_sub<result_value_type>(castLeft(*leftVar), 
                                                                             castRight(*rightVar)));
		break;

	    case OP_MUL:
		set_variant_value(*resultVar, variant_mul<result_value_type>(castLeft(*leftVar), 
                                                                             castRight(*rightVar)));
		break;

	    case OP_DIV:
		set_variant_value(*resultVar, variant_div<result_value_type>(castLeft(*leftVar), 
                                                                             castRight(*rightVar)));
		break;

            case OP_MOD:
                set_variant_value(*resultVar, variant_mod<result_value_type>(castLeft(*leftVar),
                                                                             castRight(*rightVar)));
                break;

            case OP_EXP:
                set_variant_value(*resultVar, variant_exp<result_value_type>(castLeft(*leftVar),
                                                                             castRight(*rightVar)));
                break;

	    default:
                MOOVE_THROW("unexpected opcode");
	}

        return Reply(Reply::NORMAL, resultVar);
    }
};

#if 0
/**
 * Wraps comparison operations with Variants using standard C++ operator on internal values.
 *
 * Each comparison opcode is wrapped with its C++ equivalent. Uses Moove::get_variant_value<> and Moove::set_variant_value<> to
 * retrieve values from the Variants that can be used with C++ operators. There are certain requirements for using this template:
 *      - LeftVariant and RightVariant must define a .value() method or specialize on Moove::get_variant_value<>.
 *      - ResultVariant must define a .setValue() or specialize on Moove::set_variant_value<>.
 *      - The result of the standard operation (as listed below) on the Moove::get_variant_value<> results from LeftVariant and
 *        RightVariant must result in a type for which a create() method exists on ResultVariant::classFactory().
 *
 * The opcodes are defined in the following ways:
 *      <table>
 *              <tr>
 *                      <th>Opcode</th>
 *                      <th>C++ expression</th>
 *              </tr>
 *              <tr>
 *                      <td>OP_EQ</th>
 *                      <td>variant_equal(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_NE</th>
 *                      <td>variant_not_equal(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_LT</th>
 *                      <td>variant_less_than(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_LE</th>
 *                      <td>variant_less_than_or_equal(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_GE</th>
 *                      <td>variant_greater_than_or_equal(leftVar, rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_GT</th>
 *                      <td>variant_greater_than(leftVar, rightVar)</td>
 *              </tr>
 *      </table>
 */
template<class BooleanVariant, class OperandVariant>
class UnaryComparisonOperatorDispatch : public UnaryOperatorDispatch<BooleanVariant, OperandVariant>
private:

    typedef OperatorSetBase<BooleanVariant, LeftVariant, RightVariant> Base;
    
    using Base::createResult;
    using Base::castLeft;
    using Base::castRight;

public:
    Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> var)const
    {
        MOOVE_THROW("unexpected opcode");
    }

    Reply doBinaryOp(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)const
    {
        using namespace OperatorWrappers;

        typedef typename BooleanVariant::value_type result_value_type;

        boost::shared_ptr<BooleanVariant> resultVar(createResult());

	switch(op) {
	    case OP_EQ:
                set_variant_value(*resultVar, variant_equal<result_value_type>(castLeft(*leftVar),
                                                                               castRight(*rightVar)));
		break;

	    case OP_NE:
		set_variant_value(*resultVar, variant_not_equal<result_value_type>(castLeft(*leftVar),
                                                                                   castRight(*rightVar)));
		break;

	    case OP_LT:
		set_variant_value(*resultVar, variant_less_than<result_value_type>(castLeft(*leftVar),
                                                                                   castRight(*rightVar)));
		break;

	    case OP_LE:
		set_variant_value(*resultVar, variant_less_than_or_equal<result_value_type>(castLeft(*leftVar), 
                                                                                            castRight(*rightVar)));
		break;

            case OP_GE:
                set_variant_value(*resultVar, variant_greater_than_or_equal<result_value_type>(castLeft(*leftVar), 
                                                                                               castRight(*rightVar)));
                break;

            case OP_GT:
                set_variant_value(*resultVar, variant_greater_than<result_value_type>(castLeft(*leftVar), 
                                                                                      castRight(*rightVar)));
                break;

	    default:
                MOOVE_THROW("unexpected opcode");
	}

        return Reply(Reply::NORMAL, resultVar);
    }
};

/**
 * Wraps logical operations with Variants using standard C++ operator on internal values.
 *
 * Each logical opcode is wrapped with its C++ equivalent. Uses Moove::get_variant_value<> and Moove::set_variant_value<> to 
 * retrieve values from the Variants that can be used with C++ operators. There are certain requirements for using this template:
 *      - LeftVariant and RightVariant must define a .value() method or specialize on Moove::get_variant_value<>.
 *      - ResultVariant must define a .setValue() or specialize on Moove::set_variant_value<>.
 *      - The result of the standard operation (as listed below) on the Moove::get_variant_value<> results from LeftVariant and
 *        RightVariant must result in a type for which a create() method exists on ResultVariant::classFactory().
 *
 * Also, note that unary operators are only implemented for instances of the template where LeftVariant is the same type
 * as RightVariant.
 *
 * The opcodes are defined in the following ways:
 *      <table>
 *              <tr>
 *                      <th>Opcode</th>
 *                      <th>C++ expression</th>
 *              </tr>
 *              <tr>
 *                      <td>OP_NOT</th>
 *                      <td>variant_not(var)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_OR</th>
 *                      <td>variant_equals(*leftVar, *rightVar)</td>
 *              </tr>
 *              <tr>
 *                      <td>OP_AND</th>
 *                      <td>variant_equals(*leftVar, *rightVar)</td>
 *              </tr>
 *      </table>
 */
template<class BooleanVariant, class LeftVariant = BooleanVariant, class RightVariant = LeftVariant>
class LogicOperators : public OperatorSetBase<BooleanVariant, LeftVariant, RightVariant> {
public:
    using OperatorSetBase<BooleanVariant, LeftVariant, RightVariant>::createResult;
    using OperatorSetBase<BooleanVariant, LeftVariant, RightVariant>::castLeft;
    using OperatorSetBase<BooleanVariant, LeftVariant, RightVariant>::castRight;

    typedef typename BooleanVariant::value_type result_value_type;

private:
    // called if LeftVariant is not same type as RightVariant
    Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> var, const boost::false_type&)const
    {
        MOOVE_THROW("unexpected opcode");
        return Reply();
    }

    // called if LeftVariant and RightVariant are the same type
    Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> var, const boost::true_type&)const
    {
        using namespace OperatorWrappers;

        boost::shared_ptr<BooleanVariant> resultVar(createResult());
        
        if (op == OP_NOT)
            set_variant_value(*resultVar, variant_not<result_value_type>(castLeft(*var)));
        else
            MOOVE_THROW("unexpected opcode");
        
        return Reply(Reply::NORMAL, resultVar);
    }

public:
    Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> var)const
    {
        return doUnaryOp(op, var, typename boost::is_same<LeftVariant, RightVariant>());
    }

    Reply doBinaryOp(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)const
    {
        using namespace OperatorWrappers;

        boost::shared_ptr<BooleanVariant> resultVar(createResult());

	switch(op) {
	    case OP_OR:
                set_variant_value(*resultVar, variant_or<result_value_type>(castLeft(*leftVar),
                                                                            castRight(*rightVar)));
		break;

	    case OP_AND:
		set_variant_value(*resultVar, variant_and<result_value_type>(castLeft(*leftVar),
                                                                             castRight(*rightVar)));
		break;

	    default:
                MOOVE_THROW("unexpected opcode");
	}

        return Reply(Reply::NORMAL, resultVar);
    }
};

/**
 * OperatorPackage for default builtin types.
 *
 * Template requires five paramaters:
 *      - ResultVariant is the type of Variant returned for arithmetic operations.
 *      - BooleanVariant is the type of Variant returned for comparison and logical operations.
 *      - LeftVariant is the type of Variant expected on the left-hand side of a binary operation.
 *      - RightVariant is the type of Variant expected on the right-hand side of a binary operation.
 *      - opFGMask is a bit-wise OR of the OpcodeInfo::FunctionGroup values that determine whether to include
 *        arithmetic, comparision, and/or logical operator sets into this package.
 *
 * Note that unary operations for the various operator sets are only defined if the LeftVariant and RightVariant arguments
 * are the same type.
 */
template<class ResultVariant, class BooleanVariant, class LeftVariant, class RightVariant, unsigned opFGMask>
class DefaultOperatorPackage : public OperatorPackage {
private:
    /// If opFGSelector is a non-zero value, an object is created with parent T. Otherwise, an empty class is instantiated.
    template<class T, unsigned opFGSelector>
    struct ConditionalInstantiate : public T {
    };

    template<class T>
    struct ConditionalInstantiate<T, 0> {
        Reply doUnaryOp(Opcode, std::unique_ptr<Variant>)const
        { MOOVE_THROW("unexpected opcode"); }

        Reply doBinaryOp(Opcode, std::unique_ptr<Variant>, std::unique_ptr<Variant>)const
        { MOOVE_THROW("unexpected opcode"); }
    };

    ConditionalInstantiate<ArithmeticOperators<ResultVariant, 
                                               LeftVariant, 
                                               RightVariant>, 
                           opFGMask & OpcodeInfo::FG_ARITHMETIC>        m_arithmetic;

    ConditionalInstantiate<ComparisonOperators<BooleanVariant, 
                                               LeftVariant, 
                                               RightVariant>, 
                           opFGMask & OpcodeInfo::FG_COMPARISON>        m_comparison;
    ConditionalInstantiate<LogicOperators<BooleanVariant, 
                                          LeftVariant, 
                                          RightVariant>, 
                           opFGMask & OpcodeInfo::FG_LOGIC>             m_logic;

public:
    Reply doUnaryOp(Opcode op, std::unique_ptr<Variant> var)const
    {
        switch (OpcodeInfo::getOp(op).functionalGroup()) {
            case OpcodeInfo::FG_ARITHMETIC:
                return m_arithmetic.doUnaryOp(op, var);
                
            case OpcodeInfo::FG_COMPARISON:
                return m_comparison.doUnaryOp(op, var);
                
            case OpcodeInfo::FG_LOGIC:
                return m_logic.doUnaryOp(op, var);
                
            default:
                break;
        }

        MOOVE_THROW("unexpected opcode");
        return Reply();
    }

    Reply doBinaryOp(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)const
    {
        switch (OpcodeInfo::getOp(op).functionalGroup()) {
            case OpcodeInfo::FG_ARITHMETIC:
                return m_arithmetic.doBinaryOp(op, leftVar, rightVar);

            case OpcodeInfo::FG_COMPARISON:
                return m_comparison.doBinaryOp(op, leftVar, rightVar);

            case OpcodeInfo::FG_LOGIC:
                return m_logic.doBinaryOp(op, leftVar, rightVar);
                
            default:
                MOOVE_THROW("unexpected opcode");
        }

        return Reply();
    }
};        

#endif

// Specialize the operator wrappers for the default variant types
namespace OperatorWrappers {
template<>
DefaultRealVar::value_type variant_mod(const RealVar& leftVar, const RealVar& rightVar)
{
    return std::fmod(leftVar.value(), rightVar.value());
}

template<>
DefaultRealVar::value_type variant_mod(const IntVar& leftVar, const RealVar& rightVar)
{
    return std::fmod(static_cast<DefaultRealVar::value_type>(leftVar.value()), rightVar.value());
}

template<>
DefaultRealVar::value_type variant_mod(const RealVar& leftVar, const IntVar& rightVar)
{
    return std::fmod(leftVar.value(), static_cast<DefaultRealVar::value_type>(rightVar.value()));
}

DefaultIntVar::value_type pow(DefaultIntVar::value_type x, DefaultIntVar::value_type n)
{
    DefaultIntVar::value_type result = 1;

    if (n < 0) {
        switch(x) {
            case -1:
                result = (n % 2 == 0) ? 1 : -1;
                break;

            case 0:
                result = 0;       // this should be an error (E_DIV)
                break;

            case 1:
                result = 1;
                break;

            default:
                result = 0;
                break;
        }
    } else {
        while(n > 0) {
            if(n & 1 != 0) {
                result *= x;
            }

            x *= x;
            n >>= 1;
        }
    }

    return result;
}

template<>
DefaultIntVar::value_type variant_exp(const IntVar& leftVar, const IntVar& rightVar)
{
    return pow(leftVar.value(), rightVar.value());
}

template<>
DefaultRealVar::value_type variant_exp(const IntVar& leftVar, const RealVar& rightVar)
{
    return std::pow(static_cast<DefaultRealVar::value_type>(leftVar.value()), rightVar.value());
}

template<>
DefaultRealVar::value_type variant_exp(const RealVar& leftVar, const IntVar& rightVar)
{
    return std::pow(leftVar.value(), static_cast<DefaultRealVar::value_type>(rightVar.value()));
}

template<>
DefaultIntVar::value_type variant_not(const StrVar& var)
{
    return !var.value().empty();
}

template<>
DefaultIntVar::value_type variant_or(const StrVar& leftVar, const StrVar& rightVar)
{
    return !(leftVar.value().empty() && rightVar.value().empty());
}

template<>
DefaultIntVar::value_type variant_and(const StrVar& leftVar, const StrVar& rightVar)
{
    return !(leftVar.value().empty() || rightVar.value().empty());
}

}       //namespace OperatorWrappers

// specialize ArithmeticOperator methods for strings
template<>
Reply ArithmeticOperators<DefaultStrVar, StrVar, StrVar>::doUnaryOp(Opcode, std::unique_ptr<Variant>)const
{
    MOOVE_THROW("unexpected opcode");
}

template<>
Reply ArithmeticOperators<DefaultStrVar, StrVar, StrVar>::doBinaryOp(Opcode op, 
                                                                     std::unique_ptr<Variant> leftVar, 
                                                                     std::unique_ptr<Variant> rightVar)const
{
    using namespace OperatorWrappers;

    boost::shared_ptr<DefaultStrVar> resultVar(createResult());

    if (op == OP_ADD) {
        set_variant_value(*resultVar, variant_add<StrVar::value_type>(castLeft(*leftVar),
                                                                      castRight(*rightVar)));
    } else {
        MOOVE_THROW("unexpected opcode");
    }

    return Reply(Reply::NORMAL, resultVar);
}

}       //namespace Moove

#endif
