//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_VARIANT_FACTORY_HPP
#define MOOVE_VARIANT_FACTORY_HPP

#include "except.hpp"
#include "type_registry.hpp"

#include <cstddef>
#include <boost/utility.hpp>

namespace Moove {

class Variant;

/**
 * \brief Creates instances of a Variant type
 *
 * Every Variant type added to the system by client or internal code will
 * also register a factory used for creating instances of the Variant
 * descendent it represents.
 */
class VariantFactory : boost::noncopyable {
private:
   size_t				m_instances;    ///< Number of instances of factory product in use
   const TypeRegistry::TypeEntry*	m_regEntry;

public:
   ///Thrown when a Variant object cannot be constructed from another
   typedef Exception InvalidType;

   VariantFactory() : m_instances(0), m_regEntry(0)
   {}

   virtual ~VariantFactory()
   {}

   size_t instances()const
   { return m_instances; }

   /**
    * Retrieve the TypeRegistry::TypeEntry info bound with this factory
    * \return Registry information tied with this factory
    */
   const TypeRegistry::TypeEntry& regEntry()const
   { return *m_regEntry; }

   /**
    * \brief Create a default-constructed Variant of product type
    * \return Pointer to the new object
    */
   virtual Variant* create() = 0;

#if 0
   /**
    * \brief Create a Variant using another Variant object
    * \param value A Variant object value to construct from
    * \return If successful, a pointer to the new object.
    *         Otherwise, \c 0 is returned.
    *
    * The use of this method will be primarily for reconstruction from the
    * DB synchronized value as specified by an object in a previous existence.
    */
   Variant* create(const Variant& var);
#endif

   /**
    * \internal
    * \brief Notify factory of a variant instance's creation
    * 
    * Used internally to maintain a factory's instance count. Should be called
    * automatically by Variant::Variant().
    */
   void instanceCreated()
   { ++m_instances; }

   /**
    * \internal
    * \brief Notify factory of a variant instance's destruction
    *
    * Used internally to maintain a factory's instance count. Should be called
    * automatically by Variant::~Variant().
    */
   void instanceDestroyed()
   { --m_instances; }

   /**
    * \internal
    * \brief Set the registry info tied with this factory
    * \param regEntry Registry information tied with this factory when registered
    *
    * Used by TypeRegistry when registering a type. Once the type information has been
    * saved, it will assign it to the factory using this method.
    */
   void setRegEntry(const TypeRegistry::TypeEntry& regEntry)
   { m_regEntry = &regEntry; }
};

template<class VarType>
struct InstanceCounter {
   InstanceCounter()
   { VarType::classFactory().instanceCreated(); }

   ~InstanceCounter()
   { VarType::classFactory().instanceDestroyed(); }
};

}   //namespace Moove

#endif   //MOOVE_VARIANT_FACTORY_HPP
