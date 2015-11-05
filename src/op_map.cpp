//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "op_map.hpp"
#include "op_package.hpp"

#include <algorithm>

namespace Moove {

void OperatorMap::reserve(TypeRegistry::TypeID numTypes)
{
    TypeCount oldNumTypes = typeCount();

    if(numTypes > oldNumTypes) {
        // adjust unary table
        m_unary.insert(m_unary.end(), numTypes - oldNumTypes, 0);

        // adjust binary table. Relocate old_table[x][y] to new_table[x][y]
        BinaryTable newTable(numTypes * numTypes);

        if(!m_binary.empty()) {
            for(TypeCount y = 0; y < oldNumTypes; ++y) {
                for(TypeCount x = 0; x < oldNumTypes; ++x) {
                    newTable[binaryIndex(numTypes, x, y)] = m_binary[binaryIndex(oldNumTypes, x, y)];
                }
            }
        }

        m_binary.swap(newTable);
    }
}

OperatorMap::UnaryDispatch OperatorMap::findUnary(TypeRegistry::TypeID type)const
{
    return (type < m_unary.size()) ? m_unary[type] : 0;
}

OperatorMap::BinaryDispatch OperatorMap::findBinary(TypeRegistry::TypeID left, TypeRegistry::TypeID right)const
{
    TypeCount i = binaryIndex(typeCount(), left, right);
    return (i < m_binary.size()) ? m_binary[i] : 0;
}

void OperatorMap::registerUnary(TypeRegistry::TypeID type, UnaryDispatch dispatch, bool replace)
{
    if(findUnary(type) && !replace)
        MOOVE_THROW_CLASS(DuplicatePackage, "duplicate UnaryOperator registered");

    reserve(type + 1);
    m_unary[type] = dispatch;
}

void OperatorMap::registerBinary(TypeRegistry::TypeID left, TypeRegistry::TypeID right, 
                                 BinaryDispatch dispatch,
                                 bool replace)
{
    if(findBinary(left, right) && !replace)
        MOOVE_THROW_CLASS(DuplicatePackage, "duplicate BinaryOperator registered");

    reserve(std::max(left, right) + 1);
    m_binary[binaryIndex(typeCount(), left, right)] = dispatch;
}

}	//namespace Moove
