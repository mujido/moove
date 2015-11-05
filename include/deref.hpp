//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Provides utility functors for acting on dereferenced pointers
 */
#ifndef MOOVE_DEREF_FUNCTORS_HPP
#define MOOVE_DEREF_FUNCTORS_HPP

#include <algorithm>
#include <functional>

namespace Moove {

/**
 * \brief Use another binary functor in combination with dereferened pointers
 *
 * This class is an Adaptable Binary Function</a> that takes a type 
 * and another ABF as its template paramters. The template instance will 
 * take two <tt>const T*</tt> as its arguments and return the result of the 
 * ABF taking the dereferenced values. The functionality is equivalent to:
 * \code
 * ABF()(*x, *y)
 * \endcode
 */
template<class T, class ABF>
struct BinaryDerefOp : public std::binary_function<const T*, 
						   const T*, 
						   ABF::result_type> {
   ABF::result_type operator () (const T* x, const T* y)const
   { return ABF()(*x, *y); }
};

///Standard std::less with pointer dereferencing
typedef<class T>
struct DerefLess : public BinaryDerefOp<T, std::less<T> > {
};

///Standard std::equal_to with pointer dereferencing
typedef<class T>
struct DerefEqualTo : public BinaryDerefOp<T, std::equal_to<T> > {
};

}   //namespace Moove

#endif   //MOOVE_DEREF_FUNCTORS_HPP
