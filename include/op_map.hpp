//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_OP_MAP_HPP
#define MOOVE_OP_MAP_HPP

#include "except.hpp"
#include "memory.hpp"
#include "op_package.hpp"
#include "type_registry.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace Moove {

/**
 * \brief Maps an operator from the VM depending on operands
 *
 * Using the opcode (from the CustomizableOpcode set) and operands, attempts
 * to map to an OperatorPackage object that has been previously added.
 */
class OperatorMap : boost::noncopyable {
private:
    typedef std::vector<boost::shared_ptr<OperatorPackage> > OpTable;
   
    OpTable m_unary;
    OpTable m_binary;

    OpTable::size_type binaryIndex(OpTable::size_type numTypes, OpTable::size_type left, OpTable::size_type right)const
    { return left + right * numTypes; }

    void replacePackage(OpTable& table, OpTable::size_type index, boost::shared_ptr<OperatorPackage> pkg);

public:
    typedef Exception DuplicatePackage;

    /**
     * \brief Reserve space for \a n types for unary and binary packages
     * \param numTypes Number of types to reserve space for
     */
    void reserve(TypeRegistry::TypeID numTypes);

    /**
     * \brief Lookup unary Operator object associated with given mask
     * \param opcode Opcode of operator
     * \param type Operand type
     * \return Pointer to Operator object if found. If not, \c 0.
     */
    boost::shared_ptr<OperatorPackage> findUnary(TypeRegistry::TypeID type)const;

    /**
     * \brief Lookup binary Operator object associated with given mask
     * \param opcode Opcode of operator
     * \param leftType Left-hand operand type
     * \param rightType Right-hand operand type
     * \return Pointer to Operator object if found. If not, \c 0.
     */
    boost::shared_ptr<OperatorPackage> findBinary(TypeRegistry::TypeID leftType, TypeRegistry::TypeID rightType)const;

    /**
     * \brief Installs a OperatorPackage for unary operations on the given type
     * \param type ID of the type to associate with \a pkg 
     * \param pkg OperatorPackage to handle unary operations on \a type
     * \param replace Boolean flag indicating whether or not to replace current package, if any
     * \throws DuplicatePackage if a package is already registered for \a type and
     *         <tt>replace == false</tt>
     */
    void registerUnary(TypeRegistry::TypeID type, boost::shared_ptr<OperatorPackage> pkg, bool replace = false);

    /**
     * \brief Installs a OperatorPackage for binary operations on the given types
     * \param left ID of the type to left-associate with \a pkg 
     * \param right ID of the type to right-associate with \a pkg
     * \param pkg OperatorPackage to handle binary operations on \a left and \a right
     * \param replace Boolean flag indicating whether or not to replace current package, if any
     * \throws DuplicatePackage if a package is already registered for \a left and \a right and
     *         <tt>replace == false</tt>
     */
    void registerBinary(TypeRegistry::TypeID left, TypeRegistry::TypeID right, 
                        boost::shared_ptr<OperatorPackage> pkg, 
                        bool replace = false);
};

}	//namespace Moove

#endif	//MOOVE_OP_MAP_HPP
