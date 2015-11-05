//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_STRVAR_HPP
#define MOOVE_STRVAR_HPP

#include "variant.hpp"
#include "variant_factory.hpp"

#include <string>

namespace Moove {

/**
 * \brief Abstract interface to string type
 *
 * StrVar is one abstraction required by MOOVE for its fundamental data types.
 * DefaultStrVar is a simple implementation of it.
 *
 * \warning
 * Unlike some other Variant types, the value() method on this type returns
 * reference, implying there is a std::string object somewhere saved that can
 * be referenced. This provides for less memory and time intensive work when
 * working with strings at a small cost that more dynamic string types must
 * be made separate of this interface.
 */
struct StrVar : public Variant {
    typedef std::string value_type;

    struct Factory : public VariantFactory {
        virtual StrVar* createValue(const value_type& value) = 0;
    };

    bool truthValue()const
    { return !value().empty(); }

    std::string debugStr()const;

    /**
     * \brief Retrieve value represented by Variant object
     * \return Value of represented integer
     */
    virtual const value_type& value()const = 0;

    /**
     * \brief Set the Variant object to a new representative value
     * \param value New value to assign
     */
    virtual void setValue(const value_type& value) = 0;
};

/**
 * \brief Simple implementation of StrVar
 *
 * Uses an internal std::string value as storage.
 */
class DefaultStrVar : InstanceCounter<DefaultStrVar>, public StrVar {
public:
    struct Factory : public StrVar::Factory {
        DefaultStrVar* create();

        DefaultStrVar* createValue(const value_type& str);
    };

private:
    value_type m_value;

    static Factory s_factory;
public:
    /**
     * \brief Create an empty-string DefaultStrVar object
     */
    DefaultStrVar()
    {}

    /**
     * \brief Create an DefaultStrVar object with given value
     * \param value Initialization value
     */
    DefaultStrVar(const value_type& value) : m_value(value)
    {}

    /**
     * \brief Create a copy of another StrVar object
     * \param var StrVar to copy
     *
     * Value will be set equal to \c var.value()
     */
    DefaultStrVar(const StrVar& var) : m_value(var.value())
    {}

    Factory& factory()const
    { return s_factory; }

    DefaultStrVar* clone()const;

    const value_type& value()const
    { return m_value; }

    void setValue(const value_type& value);

    static Factory& classFactory()
    { return s_factory; }
};

}       //namespace Moove

#endif   //MOOVE_INTVAR_HPP
