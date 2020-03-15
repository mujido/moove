//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_OP_MAP_HPP
#define MOOVE_OP_MAP_HPP

#include "except.hpp"
#include "memory.hpp"
#include "opcodes.hpp"
// #include "op_package.hpp"
#include "reply.hpp"
#include "type_registry.hpp"
#include "variant.hpp"

#include <memory>
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
public:
    typedef Reply (*UnaryDispatch)(Opcode, std::unique_ptr<Variant>);
    typedef Reply (*BinaryDispatch)(Opcode, std::unique_ptr<Variant>, std::unique_ptr<Variant>);

private:
    typedef std::vector<UnaryDispatch> UnaryTable;
    typedef std::vector<BinaryDispatch> BinaryTable;

    typedef UnaryTable::size_type TypeCount;
   
    UnaryTable  m_unary;
    BinaryTable m_binary;

    TypeCount typeCount()const
    { return m_unary.size(); }

    TypeCount binaryIndex(TypeCount numTypes, TypeRegistry::TypeID left, TypeRegistry::TypeID right)const
    { return left + right * numTypes; }

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
    UnaryDispatch findUnary(TypeRegistry::TypeID type)const;

    /**
     * \brief Lookup binary Operator object associated with given mask
     * \param opcode Opcode of operator
     * \param leftType Left-hand operand type
     * \param rightType Right-hand operand type
     * \return Pointer to Operator object if found. If not, \c 0.
     */
    BinaryDispatch findBinary(TypeRegistry::TypeID leftType, TypeRegistry::TypeID rightType)const;

    /**
     * \brief Installs a OperatorPackage for unary operations on the given type
     * \param type ID of the type to associate with \a pkg 
     * \param pkg OperatorPackage to handle unary operations on \a type
     * \param replace Boolean flag indicating whether or not to replace current package, if any
     * \throws DuplicatePackage if a package is already registered for \a type and
     *         <tt>replace == false</tt>
     */
    void registerUnary(TypeRegistry::TypeID type, UnaryDispatch dispatch, bool replace = false);

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
                        BinaryDispatch dispatch,
                        bool replace = false);
};

}	//namespace Moove

#endif	//MOOVE_OP_MAP_HPP
