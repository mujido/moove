//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "string_utils.hpp"

namespace Moove {

#if 0
bool StringEqualToICase::operator () (const std::string& x, const std::string& y)const
{
   return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin(), &charEqualToICase);
}

bool StringLessICase::operator () (const std::string& x, const std::string& y)const
{
   // uses .c_str() for nul characters to be used as guard characters
   std::string::size_type minSize = std::min(x.size(), y.size());
   std::pair<const char*, const char*> diff = std::mismatch(x.c_str(), x.c_str() + minSize, y.c_str());

   return charCmp(*diff.first, *diff.second);
}
   
std::size_t StringHash::operator () (const std::string& str)const
{
   std::size_t h = 0, g;

   std::string::const_iterator end = str.end();
   for(std::string::const_iterator i = str.begin(); i != end; ++i) {
      h = (h << 24) + *i;
      if((g = h & 0xF0000000))
	 h ^= g >> 24;

      h &= ~g;
   }

   return h;
}

std::size_t StringHashICase::operator () (const std::string& str)const
{
   std::size_t h = 0, g;

   std::string::const_iterator end = str.end();
   for(std::string::const_iterator i = str.begin(); i != end; ++i) {
      h = (h << 24) + std::tolower(*i);
      if((g = h & 0xF0000000))
	 h ^= g >> 24;

      h &= ~g;
   }

   return h;
}

#endif

}	// namespace Moove
