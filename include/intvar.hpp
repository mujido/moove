//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Basic Moove::Variant wrapper for the C++ \c int type.
 */
#ifndef MOOVE_INTVAR_HPP
#define MOOVE_INTVAR_HPP

#include "variant.hpp"
#include "variant_factory.hpp"

namespace Moove {

/**
 * \brief Abstract interface to integer type
 *
 * IntVar is one abstraction required by MOOVE for its fundamental data types.
 * DefaultIntVar is a simple implementation of it.
 */
struct IntVar : public Variant {
    typedef long value_type;

    /// VariantFactory for creating integer-type variants
    struct Factory : public VariantFactory {
        /**
         * \brief Create an IntVar with the given value
         * \param value Value to initialize with
         * \return An allocated object descended from IntVar
         */
        virtual IntVar* createValue(value_type value) = 0;
    };
      
    bool truthValue()const
    { return value() != 0; }

    std::string debugStr()const;

    /**
     * \brief Retrieve value represented by Variant object
     * \return Stored value.
     */
    virtual value_type value()const = 0;

    /**
     * \brief Set the Variant object to a new representative value
     * \param value New value to assign.
     */
    virtual void setValue(value_type value) = 0;
};

/**
 * \brief Simple implementation of IntVar
 *
 * Uses an internal \c int value as storage.
 */
class DefaultIntVar : public IntVar {
public:
    /**
     * \brief Simple implementation of IntVar::Factory.
     *
     * Uses operator new to allocate a new DefaultIntVar object
     */
    struct Factory : public IntVar::Factory {
        DefaultIntVar* create();
      
        DefaultIntVar* createValue(value_type value);
    };

private:
    value_type m_value;

    static Factory s_factory;

public:
    /**
     * \brief Create an uninitialized DefaultIntVar object
     *
     * The \c int value is left in an uninitialized state
     */
    DefaultIntVar();

    /**
     * \brief Create an DefaultIntVar object with given value
     * \param value Initialization value
     */
    DefaultIntVar(value_type value);

    /**
     * \brief Create a copy of another IntVar object
     * \param var IntVar to copy
     *
     * Value will be set equal to \c var.value()
     */
    DefaultIntVar(const IntVar& var);

    ~DefaultIntVar();

    Factory& factory()const
    { return s_factory; }

    DefaultIntVar* clone()const;

    value_type value()const
    { return m_value; }

    void setValue(value_type value)
    { m_value = value; }

    static Factory& classFactory()
    { return s_factory; }
};

}   //namespace Moove

#endif   //MOOVE_INTVAR_HPP

