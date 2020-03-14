//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_MEMORY_HPP
#define MOOVE_MEMORY_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <boost/type_traits.hpp>

namespace Moove {

template<class T>
struct UnaryCloner {
   T* operator () (const T* ptr)const
   { return ptr->clone(); }
};

template<class T>
struct UnaryDeleter {
   void operator () (T* ptr)const
   { delete ptr; }
};

/**
 * \brief Frees pointers in an iterator range
 * \param start Start of iterator range
 * \param end End of iterator range
 * \param deleter Function object used to delete pointers
 *
 * Calls the \c delete operator on each pointer in the range 
 * [start, end)
 */
template<class InputIterator, class Deleter>
void freeRange(InputIterator start, InputIterator end, Deleter deleter)
{
   std::for_each(start, end, deleter);
}

template<class InputIterator>
void freeRange(InputIterator start, InputIterator end)
{
   typedef typename std::iterator_traits<InputIterator>::value_type value_type;
   
   freeRange(start, end, UnaryDeleter<typename boost::remove_pointer<value_type>::type>());
}

#if 0
template<class InputIterator, class OutputIterator, class Cloner, class Deleter>
void cloneRange(InputIterator start, InputIterator end, OutputIterator out, Cloner cloner, Deleter deleter)
{
   typedef typename std::iterator_traits<OutputIterator>::value_type value_type;

   // create a vector to store temporarily for exception safety
   std::vector<value_type> temp;

   // clone all the values into temp. If excption occurs, attempt to free contents
   try {
      std::transform(start, end, std::back_insert_iterator<std::vector<value_type> >(temp), temp.begin(), cloner);
   } catch(...) {
      freeRange(temp.begin(), temp.end(), deleter);
      throw;
   }

   // made it through ok, copy to out
   std::copy(temp.begin(), temp.end(), out);
}

template<class InputIterator, class OutputIterator>
void cloneRange(InputIterator start, InputIterator end, OutputIterator out)
{
   typedef typename std::iterator_traits<InputIterator>::value_type value_type;

   cloneRange(start, end, out, UnaryCloner<value_type>(), UnaryDeleter<value_type>());
}

#endif

/**
 * \brief Properly deletes pointers in wrapped container in certain situations
 *
 * This class eases the trouble of managing containers of pointers to allocated
 * objects in the event that it is destroyed via destructor, assigned with
 * assignment operator, or AutoContainer::free() or 
 * AutoContainer::clear() are manually called.
 */
template<class Container>
struct AutoContainer : public Container {
   typedef typename Container::value_type value_type;
   typedef typename Container::iterator iterator;

   AutoContainer()
   {}

   template<class InputIterator>
   AutoContainer(InputIterator first, InputIterator last) : 
      Container(first, last)
   {}
   
   AutoContainer(AutoContainer<Container>& autoCont) : Container(autoCont)
   { autoCont.clear(); }

   ~AutoContainer()
   { freeRange(this->begin(), this->end()); }

   void autoPushFront(std::unique_ptr<value_type> ptr)
   {
      this->push_front(ptr.get());
      ptr.release();
   }

   void autoPushBack(std::unique_ptr<value_type> ptr)
   {
      this->push_back(ptr.get());
      ptr.release();
   }

   void autoErase(iterator pos)
   {
      delete *pos;
      this->erase(pos);
   }

   void autoPopFront()
   {
      if(!this->empty()) {
         delete this->front();
         this->pop_front();
      }
   }

   void autoPopBack()
   {
      if(!this->empty()) {
	 delete this->back();
         this->pop_back();
      }
   }

   void autoClear()
   {
      freeRange(this->begin(), this->end());
      this->clear();
   }

   AutoContainer<Container>& operator = (AutoContainer& autoCont)
   {
      if(this != &autoCont) {
	      this->operator = (autoCont);
	      autoCont.clear();
      }
    
      return *this;
   }
};

}     //namespace Moove

#endif   //MOOVE_MEMORY_HPP
