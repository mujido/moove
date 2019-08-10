//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Contains abstract type Moove::Variant
 */
#ifndef MOOVE_VARIANT_HPP
#define MOOVE_VARIANT_HPP

#include "variant_factory.hpp"

#include <memory>
#include <string>

namespace Moove {

//class ListVar;

/**
 * \brief Abstract Variant type
 *
 * Children of this type will implement the various data types accessible
 * from within programs. Because of the abstract methodology employed
 * within the Variant class, data types are not restricted to POD types.
 */
struct Variant {
    virtual ~Variant()
    {}
   
    /**
     * \brief Retrieve the VariantFactory object associated with this type
     * \return VariantFactory object associated with this type
     */
    virtual VariantFactory& factory()const = 0;

    virtual Variant* clone()const = 0;

    virtual bool truthValue()const = 0;

    /**
     * \brief Retrieve a string representation of this Variant's value.
     *
     * \return A string representation of the value of this Variant.
     *
     * This value is used by the disassembler to quickly retrieve a value. It can also be used for various debugging
     * activities.
     */
    virtual std::string debugStr()const = 0;
};

inline Variant* new_clone(const Variant& var)
{
    return var.clone();
}

}       //namespace Moove

#endif  //MOOVE_VARIANT_HPP
