/**
 * \file
 */
#ifndef MOOVE_BASIC_OP_PKGS_HPP
#define MOOVE_BASIC_OP_PKGS_HPP

#include "except.hpp"

#include <boost/cast.hpp>

namespace Moove {

template<class VariantType, class ValueType = typename VariantType::value_type>
struct set_value {
    void operator () (const VariantType& var, const ValueType& value)const
    { var.setValue(value); }
};

template<class ResultVariant, class GetValue, class SetValue = set_value<ResultVariant> >
class NumericOperatorPackage : public OperatorPackage {
private:
    GetValue    m_getValue;
    SetValue    m_setValue;

public:
    Reply doBinaryOp(Opcode op, std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
    {
        using namespace boost;

	shared_ptr<ResultVariant> resultVar(polymorphic_cast<ResultVariant*>(ResultVariant::classFactory().create()));
	MOOVE_ASSERT(resultVar, "invalid type");

	switch(op) {
	    case OP_ADD:
                m_setValue(*resultVar, m_getValue(*left) + m_getValue(*right));
		break;

	    case OP_SUB:
		m_setValue(*resultVar, m_getValue(*left) - m_getValue(*right));
		break;

	    case OP_MUL:
		m_setValue(*resultVar, m_getValue(*left) * m_getValue(*right));
		break;

	    case OP_DIV:
		m_setValue(*resultVar, m_getValue(*left) / m_getValue(*right));
		break;

	    default:
		return OperatorPackage::doBinaryOp(op, left, right);
	}

	return Reply(Reply::NORMAL, resultVar);
    }
};

template<class ResultVariant, class Cast, class SetValue
}       //namespace Moove

#endif
