/**
 * \file
 */
#ifndef MOOVE_DYNAMIC_ARRAY_HPP
#define MOOVE_DYNAMIC_ARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <stdexcept>

#include <boost/scoped_array.hpp>

namespace Moove {

/**
 * \brief A resizable array without the auto-expanding abilities of std::vector.
 *
 * Implements an interface to a quasi-fixed-sized array. The size can be changed explicitly with
 * resize or assignment, but otherwise the array stays a fixed size and lacks members like push_back()
 */
template<class T>
class DynamicArray {
public:
   typedef T            value_type;
   typedef T&           reference;
   typedef const T&     const_reference;
   typedef T*           iterator;
   typedef const T*     const_iterator;
   typedef ptrdiff_t    difference_type;
   typedef size_t       size_type;

   typedef std::reverse_iterator<iterator>              reverse_iterator;
   typedef std::reverse_iterator<const_iterator>        const_reverse_iterator;

private:
   boost::scoped_array<T>       m_array;
   size_t                       m_size;

   T* getAt(size_type i)
   {
      return m_array.get() ? &m_array[i] : 0;
   }

   const T* getAt(size_type i)const
   {
      return m_array.get() ? &m_array[i] : 0;
   }

public:
   DynamicArray() : m_array(0), m_size(0)
   {}

   DynamicArray(const DynamicArray& arr) : m_array(0), m_size(0)
   {
      resize(arr.m_size);
      std::copy(arr.begin(), arr.end(), begin());
   }

   DynamicArray(size_type n, value_type value = T()) : m_array(0), m_size(0)
   {
      resize(n);
      std::fill(begin(), end(), value);
   }

   iterator begin()
   { return getAt(0); }

   iterator end()
   { return getAt(m_size); }

   const_iterator begin()const
   { return getAt(0); }

   const_iterator end()const
   { return getAt(m_size); }

   size_type size()const
   { return m_size; }

   bool empty()const
   { return m_size == 0; }

   size_type max_size()const
   { return std::numeric_limits<size_type>::max(); }

   void resize(size_type n)
   {
      boost::scoped_array<value_type> tmp_array(new value_type[n]);
      
      std::copy(begin(), begin() + std::min(m_size, n), &tmp_array[0]);
      m_array.swap(tmp_array);
      m_size = n;
   }
          
   void swap(DynamicArray& arr)
   {
      m_array.swap(arr.m_array);
      std::swap(m_size, arr.m_size);
   }

   T& at(size_type i)
   {
      if(i >= m_size) 
         throw std::out_of_range("Index out of range");

      return m_array[i];
   }

   const T& at(size_type i)const
   {
      if(i >= m_size) 
         throw std::out_of_range("Index out of range");

      return m_array[i];
   }

   T& operator [] (size_type i)
   { return m_array[i]; }

   const T& operator [] (size_type i)const
   { return m_array[i]; }

   DynamicArray& operator = (const DynamicArray& arr)
   {
      if (this != &arr) {
         boost::scoped_array<value_type> tmp_array(new value_type[arr.m_size]);
         
         std::copy(arr.begin(), arr.end(), &tmp_array[0]);
         m_array.swap(tmp_array);
         m_size = arr.m_size;
      }

      return *this;
   }
};

template<class T>
bool operator == (const DynamicArray<T>& lhs, const DynamicArray<T>& rhs)
{ return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()); }

template<class T>
bool operator != (const DynamicArray<T>& lhs, const DynamicArray<T>& rhs)
{ return !(lhs == rhs); }

template<class T>
bool operator < (const DynamicArray<T>& lhs, const DynamicArray<T>& rhs)
{ return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

}       //namespace Moove

#endif  //MOOVE_DYNAMIC_ARRAY_HPP
      
   
