//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_EXCEPT_HPP
#define MOOVE_EXCEPT_HPP

#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <string>

// #define MOOVE_ASSERT_USE_PRETTY_FUNCTION

#ifdef MOOVE_ASSERT_USE_PRETTY_FUNCTION
# define MOOVE_ASSERT_FUNCTION  __PRETTY_FUNCTION__
#elif defined(MOOVE_ASSERT_USE_STD_FUNC)
# define MOOVE_ASSERT_FUNCTION  __func__
#elif defined(MOOVE_ASSERT_USE_FUNCTION)
# define MOOVE_ASSERT_FUNCTION  __FUNCTION__
#else 
# define MOOVE_ASSERT_FUNCTION "<unknown>"
#endif

#define MOOVE_THROW_CLASS(exceptClass, msg) \
    throw exceptClass(__FILE__, __LINE__, MOOVE_ASSERT_FUNCTION, msg)

#define MOOVE_THROW(msg) \
    MOOVE_THROW_CLASS(Exception, msg)
    
#define MOOVE_ASSERT(assertion, msg) \
    if(!(assertion)) \
        MOOVE_THROW(msg)

namespace Moove {

class Exception : public std::exception {
private:
   std::string m_msg;

public:
   Exception(const std::string& msg) throw() : m_msg(msg)
   {}

   Exception(const std::string& className,
             const std::string& methodName,
             const std::string& msg) throw()
   {
      m_msg = ("Exception in Moove::" + className + "::" + 
               methodName + "(): " + msg);
   }

   Exception(const char *file, std::size_t line, const char *func, const std::string& msg)
   {
      std::ostringstream str;

      str << "MOOVE exception in " << file << ':' << line << ", " << func << ": " << msg;
      m_msg = str.str();
   }

   ~Exception() throw()
   {}

   const char* what()const throw()
   { return m_msg.c_str(); }
};

}   //namespace Moove

#endif   //MOOVE_EXCEPT_HPP
               
