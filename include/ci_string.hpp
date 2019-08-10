//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_CI_STRING_HPP
#define MOOVE_CI_STRING_HPP

#include <cctype>
#include <string>

namespace Moove {

template<class CharT>
struct ci_char_traits : public std::char_traits<CharT> {
   static bool eq(const CharT& c1, const CharT& c2)
   {
      return std::tolower(c1) == std::tolower(c2);
   }

   static bool ne(const CharT& c1, const CharT& c2)
   {
      return std::tolower(c1) != std::tolower(c2);
   }

   static bool lt(const CharT& c1, const CharT& c2)
   {
      return std::tolower(c1) < std::tolower(c2);
   }

   static int compare(const CharT* s1, const CharT* s2, size_t n)
   {
      for(size_t i = 0; i < n; ++i)
	 if(ne(s1[i], s2[i]))
	    return lt(s1[i], s2[i]) ? -1 : 1;

      return 0;
   }

   static const CharT* find(const CharT* p, std::size_t n, 
				const CharT& c)
   {
      while(n-- && ne(*p, c))
	 ++p;

      return p;
   }
};

///Perform a case-insensitive character comparison (less-than)
struct ltchari : public std::binary_function<char, char, bool> {
   bool operator() (char x, char y)const
   { return std::tolower(x) < std::tolower(y); }
};

///Perform a case-insensitive character comparison (equal-to)
struct eqchari : public std::binary_function<char, char, bool> {
   bool operator() (char x, char y)const
   { return std::tolower(x) == std::tolower(y); }
};

typedef std::basic_string<char, ci_char_traits<char> > ci_string;
typedef std::basic_string<wchar_t, ci_char_traits<wchar_t> > ci_wstring;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& s,
	     const std::basic_string<CharT, ci_char_traits<CharT> >& str)
{
   return s << str.c_str();
}

}   //namespace Moove

#endif   //MOOVE_CI_STRING_HPP
   
