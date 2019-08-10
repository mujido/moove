/**
 * \file
 */
#ifndef MOOVE_OP_WRAPPERS_HPP
#define MOOVE_OP_WRAPPERS_HPP

#include <cmath>

namespace Moove {

/**
 * Retrieve the internal value of a Variant object.
 *
 * Used by OperatorSet classes and DefaultOperatorPackage to retrieve a value from a variant that can be used by
 * the OperatorWrappers for evaluating new values.
 *
 * The standard implementation uses <tt>variant.value()</tt> to retrieve this value. VariantType must define a type
 * <tt>VariantType::value_type</tt>, which is the type returned by <tt>variant.value()</tt>.
 *
 * \sa set_variant_value(), ArithmeticOperatorSet, ComparisonOperatorSet, LogicOperatorSet, and DefaultOperatorPackage.
 */
template<class VariantType>
typename VariantType::value_type get_variant_value(const VariantType& var)
{ 
    return var.value(); 
}

/**
 * Set the internal value of a Variant object.
 *
 * Used by OperatorSet classes and DefaultOperatorPackage to set a value from a variant that can be used by
 * the OperatorWrappers for evaluating new values.
 *
 * The standard implementation uses <tt>variant.setValue()</tt> to retrieve this value. VariantType must define a type
 * <tt>VariantType::value_type</tt>, which is the type accepted as an argument by <tt>variant.setValue()</tt>.
 *
 * \sa get_variant_value(), ArithmeticOperatorSet, ComparisonOperatorSet, LogicOperatorSet, and DefaultOperatorPackage.
 */
template<class VariantType>
void set_variant_value(VariantType& var, const typename VariantType::value_type& value)
{ 
    var.setValue(value); 
}

/// Contains wrappers around the C++ operators and function calls to provide the Moove operator functionality.
namespace OperatorWrappers {

template<class ResultType, class OperandVariant>
ResultType variant_negate(const OperandVariant& var)
{ return -get_variant_value(var); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_add(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) + get_variant_value(rightVar); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_sub(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) - get_variant_value(rightVar); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_mul(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) * get_variant_value(rightVar); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_div(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) / get_variant_value(rightVar); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_mod(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) % get_variant_value(rightVar); }

template<class ResultType, class LeftVariant, class RightVariant>
ResultType variant_exp(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return std::pow(get_variant_value(leftVar), get_variant_value(rightVar)); }

template<class BooleanType, class OperandVariant>
BooleanType variant_not(const OperandVariant& var)
{ return !get_variant_value(var); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_or(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) || get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_and(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) && get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_equal(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) == get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_not_equal(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) != get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_less_than(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) < get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_less_than_or_equal(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) <= get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_greater_than_or_equal(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) >= get_variant_value(rightVar); }

template<class BooleanType, class LeftVariant, class RightVariant>
BooleanType variant_greater_than(const LeftVariant& leftVar, const RightVariant& rightVar)
{ return get_variant_value(leftVar) > get_variant_value(rightVar); }

}       // namespace OperatorWrappers

}       // namespace Moove

#endif
