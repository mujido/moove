//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_VARIANT_ID_HPP
#define MOOVE_VARIANT_ID_HPP

#include "type_registry.hpp"

namespace Moove {

class Variant;

///Represent a Variant's dynamic type with a lightweight object
class VariantID {
private:
   const TypeRegistry::Info* m_info;

public:
   /** 
    * \brief Create an invalid VariantID object
    *
    * This object will be invalid in the sense that <tt>valid() == false</tt>.
    */
   VariantID() : m_info(0)
   {}

   /**
    * \brief Create a VariantID object for the given Variant object
    * \param var A Variant object
    * 
    * The object created by this constructor will be unique to the dynamic
    * type of the \a var object. Example:
    * \code
    * VariantID(IntVar(1)) == VariantID(IntVar(2))
    * VariantID(IntVar(1)) != VariantID(RealVar(1))
    * \endcode
    */
   explicit VariantID(const Variant& var);
   
   /**
    * \brief Create a VariantID object from the std::type_info for a Variant
    * \param info TypeRegistry::Info object associated with this VariantID
    *
    * \warning
    * The given \a info object will be referenced internally throughout the
    * lifetime of this VariantID object.
    */
   explicit VariantID(const TypeRegistry::Info& info) : m_info(&info)
   {}

   /**
    * \brief Determine the validity of a VariantID object
    * \return If the object represents a valid Variant type, \c true.
    *         Otherwise, \c false.
    */
   operator bool ()const
   { return m_info != 0; }

   VariantID& operator = (const VariantID& id);
};

inline bool operator == (const VariantID& x, const VariantID& y)
{ return x.m_info == y.m_info; }

inline bool operator != (const VariantID& x, const VariantID& y)
{ return x.m_info != y.m_info; }

inline bool operator < (const VariantID& x, const VariantID& y)
{ return x.m_info < y.m_info; }

}   //namespace Moove

#endif   //MOOVE_VARIANT_ID_HPP
