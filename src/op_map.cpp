//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "op_map.hpp"
#include "op_package.hpp"

#include <algorithm>

namespace Moove {

void OperatorMap::replacePackage(OpTable& table, OpTable::size_type index, boost::shared_ptr<OperatorPackage> pkg)
{
    MOOVE_ASSERT(index < table.size(), "type index out of range");

    table[index] = pkg;
}

void OperatorMap::reserve(TypeRegistry::TypeID numTypes)
{
    OpTable::size_type oldNumTypes = m_unary.size();

    if(numTypes > oldNumTypes) {
        // adjust unary table
        m_unary.insert(m_unary.end(), numTypes - oldNumTypes, boost::shared_ptr<OperatorPackage>());

        // adjust binary table. Relocate old_table[x][y] to new_table[x][y]
        OpTable newTable(numTypes * numTypes);

        if(!m_binary.empty()) {
            for(OpTable::size_type y = 0; y < oldNumTypes; ++y) {
                for(OpTable::size_type x = 0; x < oldNumTypes; ++x) {
                    newTable[binaryIndex(numTypes, x, y)] = m_binary[binaryIndex(oldNumTypes, x, y)];
                }
            }
        }

        m_binary.swap(newTable);
    }
}

boost::shared_ptr<OperatorPackage> OperatorMap::findUnary(TypeRegistry::TypeID type)const
{
    return (type < m_unary.size()) ? m_unary[type] : boost::shared_ptr<OperatorPackage>();
}

boost::shared_ptr<OperatorPackage> OperatorMap::findBinary(TypeRegistry::TypeID left, TypeRegistry::TypeID right)const
{
    OpTable::size_type i = binaryIndex(m_unary.size(), left, right);
    return (i < m_binary.size()) ? m_binary[i] : boost::shared_ptr<OperatorPackage>();
}

void OperatorMap::registerUnary(TypeRegistry::TypeID type, boost::shared_ptr<OperatorPackage> pkg, bool replace)
{
    if(findUnary(type) && !replace)
        MOOVE_THROW_CLASS(DuplicatePackage, "duplicate OperatorPackage registered");

    reserve(type + 1);
    replacePackage(m_unary, type, pkg);
}

void OperatorMap::registerBinary(TypeRegistry::TypeID left, TypeRegistry::TypeID right, 
                                 boost::shared_ptr<OperatorPackage> pkg, 
                                 bool replace)
{
    if(findBinary(left, right) && !replace)
        MOOVE_THROW_CLASS(DuplicatePackage, "duplicate OperatorPackage registered");

    reserve(std::max(left, right) + 1);
    replacePackage(m_binary, binaryIndex(m_unary.size(), left, right), pkg);
}

}	//namespace Moove
