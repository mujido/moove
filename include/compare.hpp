//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_COMPARE_HPP
#define MOOVE_COMPARE_HPP

namespace Moove {

/**
 * \brief Performs a 3-way compare on the operands
 * \param x Left hand side of compare
 * \param y Right hand side of compare
 * \return A value when compared to zero represents the relation of x to y.
 *
 * \a x and \a y must be Less-Than Comparable.
 */
template<class T>
int compare_3way(const T& x, const T& y)
{
   if(x < y)
      return -1;
   else if(y < x)
      return 1;
   else
      return 0;
}

/**
 * \brief Performs a 3-way compare on the operands
 * \param x Left hand side of compare
 * \param y Right hand side of compare
 * \param comp A binary predicate that implements Strict Weak Ordering (less)
 * \return A value when compared to zero represents the relation of x to y.
 */
template<class T, class StrictWeakBinaryPred>
int compare_3way(const T& x, const T& y, StrictWeakBinaryPred comp)
{
   if(comp(x, y))
      return -1;
   else if(comp(y, x))
      return 1;
   else
      return 0;
}

}    //namespace Moove

#endif   //MOOVE_COMPARE_HPP
