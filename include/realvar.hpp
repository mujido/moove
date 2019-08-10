//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Basic Moove::Variant wrapper for the C++ \c double type.
 */
#ifndef MOOVE_REALVAR_HPP
#define MOOVE_REALVAR_HPP

#include "variant.hpp"
#include "variant_factory.hpp"

namespace Moove {

/**
 * \brief Abstract interface to real (C++ double) type
 *
 * RealVar is one abstraction requird by MOOVE for its fundamental data types.
 * DefaultRealVar is a simple implementation of it.
 */
struct RealVar : public Variant {
    typedef double value_type;

    struct Factory : public VariantFactory {
	virtual RealVar* createValue(const value_type& value) = 0;
    };

    bool truthValue()const
    { return value() != 0; }

    std::string debugStr()const;

    /**
     * \brief Retrieve value represented by Variant object
     * \return Value contained within the object
     */
    virtual double value()const = 0;

    /**
     * \brief Set the Variant object to a new representative value
     * \param value New value to assign
     */
    virtual void setValue(const value_type& value) = 0;
};
   
/**
 * \brief Simple implementation of RealVar
 */
class DefaultRealVar : public RealVar {
public:
    struct Factory : public RealVar::Factory {
	DefaultRealVar* create();
      
	DefaultRealVar* createValue(const value_type& value);
    };

private:
    value_type m_value;

    static Factory s_factory;

public:
    /**
     * \brief Create an uninitialized DefaultRealVar object
     *
     * The value is left in an uninitialized state
     */
    DefaultRealVar();

    /**
     * \brief Create an DefaultRealVar object with given value
     * \param value Initialization value
     */
    DefaultRealVar(const value_type& value);

    /**
     * \brief Create a copy of another RealVar object
     * \param var RealVar to copy
     *
     * Value will be set equal to \c var.value()
     */
    DefaultRealVar(const RealVar& var);

    ~DefaultRealVar();

    Factory& factory()const
    { return s_factory; }

    DefaultRealVar* clone()const;

    double value()const
    { return m_value; }

    void setValue(const value_type& value)
    { m_value = value; }

    static Factory& classFactory()
    { return s_factory; }
};

}   //namespace Moove

#endif   //MOOVE_REALVAR_HPP

