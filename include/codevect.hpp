//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_CODEVECT_HPP
#define MOOVE_CODEVECT_HPP

#include "dynamic_array.hpp"

namespace Moove {

///Contains the bytecode for a single code vector
class CodeVector : public DynamicArray<unsigned char> {
public:
   typedef unsigned char     Byte;
   typedef size_type         Word;

private:
   static bool checkPackSize(Word n);

public:
   CodeVector()
   {}

   CodeVector(Word size) : DynamicArray<Byte>(size)
   {}

   /**
    * \brief Pack a Word value and store into vector
    * \param pos Where in vector to store packed word
    * \param value Value to be packed and inserted
    * \param n Number of bytes to attempt to pack into
    * \throws PackError if \a value is too large to fit in \a n bytes
    *
    * Attempts to pack \a value into \a n bytes and insert them into the code
    * vector.
    */
   void packWord(iterator pos, Word value, Word n);

   /**
    * \brief Unpack a Word value and return it
    * \param pos Iterator to beginning position of Word
    * \param n Number of bytes Word is packed into
    * \return Value of unpacked Word
    * \exception UnpackError if pos + n extends past vector
    * \warning If \a n is too large, value may overflow
    * 
    * Attempts to unpack a previously packed Word at \a pos and return the value.
    */
   Word unpackWord(const_iterator pos, Word n)const;

   /**
    * \brief Calculate number of bytes required to store a label in this code vector
    * \return Number of bytes required to store a label in this code vector
    */
   Word labelSize()const
   { return packedWordSize(size() - 1); }

   /**
    * \brief Calculate packed size of a Word value
    * \param value A Word value to be measured for packing
    * \return Number of bytes required by packed form of \a value
    *
    * Word values can often be packed into small numbers. This function
    * deterimines how small the given \a value can be packed.
    */
   static Word packedWordSize(Word value);
};

}   //namespace Moove

#endif    //MOOVE_CODEVECT_HPP
