//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_MEM_POOL_HPP
#define MOOVE_MEM_POOL_HPP

#include "memory.hpp"

#include <algorithm>
#include <memory>
#include <vector>
#include <boost/utility.hpp>

namespace Moove {
   
template<class BaseType>
class MemoryPool : boost::noncopyable {
private:
   typedef AutoContainer<std::vector<BaseType*> > Pool;
   typedef typename Pool::iterator                iterator;
   typedef typename Pool::const_iterator          const_iterator;

   Pool m_pool;

   iterator find(const BaseType* obj)
   { return std::find(m_pool.begin(), m_pool.end(), obj); }

   const_iterator find(const BaseType* obj)const
   { return std::find(m_pool.begin(), m_pool.end(), obj); }
   
public:
   void insert(std::auto_ptr<BaseType> obj)
   {
      if(obj.get()) {
	 m_pool.push_back(obj.get());
	 obj.release();
      }
   }

   void remove(const BaseType* obj)
   {
      iterator pos = find(obj);
      if(pos != m_pool.end())
	 m_pool.erase(pos);
   }
	 
   void clear(bool free = false)
   {
      if(free)
	 m_pool.autoClear();
      else
	 m_pool.clear();
   }
};

}    //namespace Moove

#endif   //MOOVE_MEM_POOL_HPP
