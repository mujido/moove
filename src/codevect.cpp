#include "codevect.hpp"

#include <cassert>
#include <limits>

namespace Moove {

bool CodeVector::checkPackSize(Word n)
{
   // ensures that n > 0 and that the number of bits does not exceed the range
   // of the Word type
   return n > 0 && 8*n <= std::numeric_limits<Word>::digits;
}

void CodeVector::packWord(iterator pos, Word value, Word n)
{
   assert(checkPackSize(n));

   // convert word to bytes in big-endian format
   for(; n > 0; --n, ++pos) {
      *pos = static_cast<Byte>(value);
      value >>= 8;
   }
}

CodeVector::Word CodeVector::unpackWord(const_iterator pos, Word n)const
{
   assert(checkPackSize(n));

   Word value = 0;

   // read back bytes in big-endian order (move backwards)
   pos += n - 1;
   while(n--)
      value = (value << 8) | *pos--;
      
   return value;
}

CodeVector::Word CodeVector::packedWordSize(Word value)
{
   Word n;

   for(n = 0; value != 0; ++n)
      value >>= 8;

   return n ? n : 1;
}

}	//namespace Moove
      
