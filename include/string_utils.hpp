//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_STRING_UTILS_HPP
#define MOOVE_STRING_UTILS_HPP

#include <algorithm>
#include <cctype>
#include <string>

namespace Moove {

#if 0

typedef std::equal_to<std::string> StringEqualTo;
typedef std::less<std::string>     StringLess;

struct StringEqualToICase {
   static bool charEqualToICase(char x, char y)
   { return std::tolower(x) == std::tolower(y); }

   bool operator () (const std::string& x, const std::string& y)const;
};

struct StringLessICase {
   static bool charLessICase(char x, char y)
   { return std::tolower(x) < std::tolower(y); }
   
   bool operator () (const std::string& x, const std::string& y)const;
};

#endif

struct iless {
   static bool charCmp(char x, char y)
   { return tolower(x) < tolower(y); }

   bool operator () (const std::string& s1, const std::string& s2)const
   { return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), &charCmp); }
};
      
#if 0

struct StringHash {
   std::size_t operator () (const std::string& str)const;
};

struct StringHashICase {
   std::size_t operator () (const std::string& str)const;
};

#endif

}   //namespace Moove

#endif   //MOOVE_STRING_UTILS_HPP
