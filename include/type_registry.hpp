//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_TYPE_REGISTRY_HPP
#define MOOVE_TYPE_REGISTRY_HPP

#include "except.hpp"
// #include "op_package.hpp"

#include <deque>
#include <iterator>
#include <set>
#include <string>
#include <boost/utility.hpp>

namespace Moove {

class VariantFactory;

/**
 * \brief Maintain a registry of variant types
 *
 * An instance of this object will maintain a mapping of types from
 * their package name to a fixed value to fascilitate in their use within
 * MOOVE system and synchronization to DB. 
 *
 * If a type is unable to be loaded when needed, an error recovery mechanism
 * will take over, preserving the data necessary to construct the object
 * again later in a mediator variant type, UnregisteredVar.
 */
class TypeRegistry : boost::noncopyable {
public:
   class TypeEntry;

private:
   struct TypeEntryNameLessThan {
      bool operator () (const TypeEntry& x, const TypeEntry& y)const;
   };

   typedef std::set<TypeEntry, TypeEntryNameLessThan>	Registry;

   typedef std::deque<const TypeEntry*>			TypeEnumerator;

public:
   ///Identifies a type numerically. Linked with VariantFactory
   typedef TypeEnumerator::size_type			TypeID;

   class TypeEntry {		// XXX used to be noncopyable. think about this some
   private:
      std::string		m_name;
      TypeID			m_id;
      VariantFactory*		m_factory;

   public:
      /**
       * \internal
       * \brief Create a TypeEntry object with only a name associated with it
       * \param name Name to assign
       *
       * This constructor is used internally for searching for other TypeEntry objects by name.
       */
      TypeEntry(const std::string& name) : m_name(name)
      {}

      TypeEntry(const std::string& name, TypeID id, VariantFactory& factory) : m_name(name), m_id(id), m_factory(&factory)
      {}

      const std::string& name()const
      { return m_name; }

      TypeID id()const
      { return m_id; }

      VariantFactory& factory()const
      { return *m_factory; }

      operator TypeID ()const
      { return m_id; }
   };

private:
   Registry       m_registry;		///< Used to quickly locate types by name
   TypeEnumerator m_enumerator;         ///< Used to enumerate types for TypeID and for constant lookup time by TypeID

public:
   /// Iterator type for registered types
   class const_iterator {
   private:
      TypeEnumerator::const_iterator m_iter;

   public:
      typedef std::bidirectional_iterator_tag	iterator_category;
      typedef TypeEntry				value_type;
      typedef ptrdiff_t				difference_type;
      typedef TypeEntry*			pointer;
      typedef TypeEntry&			reference;

      explicit const_iterator(TypeEnumerator::const_iterator iter) : m_iter(iter)
      {}

      const TypeEntry& operator * ()const
      { return **m_iter; }

      const TypeEntry* operator -> ()const
      { return *m_iter; }

      const_iterator& operator ++ ()
      {
	 ++m_iter;
	 return *this;
      }

      const_iterator& operator -- ()
      { 
	 --m_iter;
	 return *this;
      }

      const_iterator operator ++ (int)
      { return const_iterator(m_iter++); }

      const_iterator operator -- (int)
      { return const_iterator(m_iter--); }

      bool operator == (const_iterator iter)const
      { return m_iter == iter.m_iter; }

      bool operator != (const_iterator iter)const
      { return m_iter != iter.m_iter; }
   };
	 
   ///Thrown if a type is registered twice
   typedef Exception			DuplicateType;

   ///Thrown if an OperatorPackage is registered twice for same type
   typedef Exception			DuplicatePackage;

   ///Thrown if an OperatorPackage is registered for invalid type IDs
   typedef Exception			InvalidType;

   /**
    * \brief Retrieve number of types registered
    * \return Number of types currently registered
    */
   TypeID typeCount()const
   { return m_enumerator.size(); }

   /**
    * \brief Retrieve beginning iterator over registered types
    * \return Iterator to first type in set of registered types
    * 
    * \warning
    * No guarantee is given for the ordering of types
    */
   const_iterator begin()const
   { return const_iterator(m_enumerator.begin()); }

   /**
    * \brief Retrieve end iterator over registered types
    * \return Iterator to end of set of registered types
    *
    * \warning
    * No guarantee is given for the ordering of types
    */
   const_iterator end()const
   { return const_iterator(m_enumerator.end()); }

   /**
    * \brief Find registration info for a variant type by name
    * \param name Type name as formally registered
    * \return If successful, pointer to a TypeEntry object\n
    *         If not, \c 0
    */
   const TypeEntry* findType(const std::string& name)const;

   /**
    * \brief Find registration info for a variant type by its TypeID
    * \param id TypeID assigned to the type
    * \return If successful, pointer to a TypeEntry object\n
    *         If not, \c 0
    */
   const TypeEntry* findType(TypeID id)const;

#if 0
   /**
    * \brief Find a unary operator package by TypeID
    * \param id Type associated with the operator package
    * \return If successful, pointer to an OperatorPackage\n
    *         If not, \c 0
    */
   OperatorPackage* findUnaryOpPkg(TypeID id)const;

   /**
    * \brief Find a binary operator package by TypeID values
    * \param leftID Left-hand type associated with the operator package
    * \param rightID Right-hand type associated with the operator package
    * \return If successful, pointer to an OperatorPackage\n
    *         If not, \c 0
    */
   OperatorPackage* findBinaryOp(TypeID leftID, TypeID rightID)const;

   /**
    * \brief Register an operator package for unary operations
    * \param pkg OperatorPackage to register
    * \param id Type to register for
    * \param replace Whether or not to replace a package already 
    *                associated with the factory
    * \throws InvalidType if \a id is invalid
    * \throws DuplicatePackage if a package is associated with id
    *         already and <tt>replace == false</tt>
    */
   void registerUnaryOpPkg(OperatorPackage pkg, 
			   TypeID id,
			   bool replace = false);

   /**
    * \brief Register an operator package for binary operations
    * \param pkg OperatorPackage to associate
    * \param leftID Left-hand type to associate
    * \param rightID Right-hand type to associate
    * \param symmetric Is the operation symmetric (see below)
    * \param replace Whether or not to replace a package already 
    *                associated with the factory
    * \throws InvalidType if \a id is invalid
    * \throws DuplicatePackage if a package is associated with id
    *         already and <tt>replace == false</tt>
    *
    * \note
    * An operation is symmetric if <tt>value1 \<op\> value2</tt> is equivalent
    * to <tt>value2 \<op\> value1</tt>.
    */
   void registerBinaryOpPkg(const OperatorPackage& pkg, 
			    TypeID leftID,
			    TypeID rightID,
			    bool symmetric = false,
			    bool replace = false);
#endif

   /**
    * \brief Register a type formally by its factory
    * \param factory Type's factory
    * \param replace Whether or not to replace a type with the same name
    * \exception DuplicateType if a type with the same name was formerly
    *            registered and <tt>replace == false</tt>
    */
   const TypeEntry& registerType(const std::string& name, VariantFactory& factory, bool replace = false);
};

}   //namespace Moove

#endif   //MOOVE_TYPE_REGISTRY_HPP

