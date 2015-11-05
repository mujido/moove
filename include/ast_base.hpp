//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Definitions for use in general in AST parsing areas
 */
#ifndef MOOVE_AST_BASE_HPP
#define MOOVE_AST_BASE_HPP

#include "mem_pool.hpp"
#include "memory.hpp"

namespace Moove {

///Abstract base for all AST memory pooling
struct ASTPoolObject {
   virtual ~ASTPoolObject()
   {}
};

//Template for AutoContainer to be used in ASTMemPool
template<class T>
struct ASTAutoContainer : public ASTPoolObject, 
			  public AutoContainer<T> {
};

template<class T>
struct ASTPoolPtr : public ASTPoolObject, public std::auto_ptr<T> {
   explicit ASTPoolPtr(T* ptr = 0) : std::auto_ptr<T>(ptr)
   {}

   ASTPoolPtr(ASTPoolPtr<T>& ptr) : std::auto_ptr<T>(ptr)
   {}

   template<class X>
   ASTPoolPtr(std::auto_ptr<X>& ptr) : std::auto_ptr<T>(ptr)
   {}

   ASTPoolPtr<T>& operator = (std::auto_ptr<T>& ptr)
   {
      std::auto_ptr<T>::operator = (ptr);
      return *this;
   }

   template<class X>
   ASTPoolPtr<T>& operator = (std::auto_ptr<X>& ptr)
   {
      std::auto_ptr<T>::operator = (ptr);
      return *this;
   }
};

typedef MemoryPool<ASTPoolObject> ASTMemPool;

}   //namespace Moove

#endif   //MOOVE_AST_BASE_HPP
