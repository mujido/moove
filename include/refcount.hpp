//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_REFCOUNT_HPP
#define MOOVE_REFCOUNT_HPP

#include <memory>

namespace Moove {

/**
 * \brief Implements a ref-counted pointer
 *
 * Each time another instance of this class is initialized with another,
 * the count associated with its pointer will be incremented. Likewise, when
 * released, the count will drop. Once count drops to 0, it will be deleted.
 * Proper refcounts are only kept when referenced through a RefCountPtr object.
 * \code
 * RefCountPtr<int>* intPtr = new RefCountPtr<int>(new int);
 * **intPtr = 0;
 * RefCountPtr<int> intPtr2 = intPtr;
 * delete intPtr;
 * //*intPtr2 still valid
 * \endcode
 */
template<class T>
class RefCountPtr {
private:
   struct Data {
      std::auto_ptr<T> ptr;
      unsigned long    count;

      explicit Data(T* a_ptr) : ptr(a_ptr), count(1)
      {}
   };

   Data* m_data;

   void release()
   {
      if(!--m_data->m_count) {
	 delete m_data;
	 m_data = 0;       //will force null exception if misused
      }
   }

public:
   explicit RefCountPtr(T* ptr = 0) 
   { m_data = new Data(ptr); }

   RefCountPtr(RefCountPtr<T>& ref) : m_data(ref.m_data)
   { ++m_data->count; }

   ~RefCountPtr()
   { release(); }

   unsigned int refCount()const
   { return m_data->count; }

   const T& operator * ()const
   { return *m_data->ptr.get(); }

   T& operator * ()
   { return *m_data->ptr.get(); }

   const T* operator -> ()const
   { return m_data->ptr.get(); }

   T* operator -> ()
   { return m_data->ptr.get(); }

   void reset(T* ptr = 0)
   { 
      release(); 
      m_data.reset(new Data(ptr));
   }

   RefCountPtr<T>& operator = (const RefCountPtr<T>& ref)
   {
      if(this != &ref) {
	 release();
	 m_data = ref.m_data;
	 ++m_data->count;
      }

      return *this;
   }
};

}   //namespace Moove

#endif   //MOOVE_REFCOUNT_HPP

