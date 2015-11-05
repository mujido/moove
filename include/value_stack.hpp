//Copyright (c) 2005 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_VALUE_STACK_HPP
#define MOOVE_VALUE_STACK_HPP

#include <boost/ptr_container/ptr_vector.hpp>

namespace Moove {

class ValueStack {
private:
    typedef boost::ptr_vector<Variant> ValueArray;

    ValueArray  m_values;

public:
    const Variant& operator [] (ValueArray::size_type i)const;

    
#endif
