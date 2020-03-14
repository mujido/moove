//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_LINKER_REF_HPP
#define MOOVE_LINKER_REF_HPP

#include "codevect.hpp"

#include <array>

namespace Moove {

/**
 * \brief An immediate value encoded directly into bytecode
 *
 * Objects of this class are used by the Compiler to emit references to
 * labels, literals, variables, and forks for later insertion into CodeVect
 * objects. Allows for easier code labeling and word packing.
 */
class ImmediateValue {
public:
   ///Type of immediate value
   enum Type {
      INTEGER,                  ///< Packed unsigned integer
      LABEL,                    ///< Label identifier
      LITERAL,                  ///< Literal identifier
      TEMP,			///< Variable/temporary identfier
      FORK                      ///< Fork code vector identifier
   };

   static const unsigned NUM_TYPES = 5;

   /// Stores an array of deltas indexed by ImmediateValue::Type for label computation.
   typedef std::array<CodeVector::difference_type, NUM_TYPES>		Deltas;

private:
   CodeVector::Word m_pos;
   Type             m_type;
   CodeVector::Word m_value;

   // label-specific values used for re-calculating offsets with varying-size immediate values
   Deltas m_immDeltas;

public:
   ImmediateValue(CodeVector::Word pos, Type type, CodeVector::Word value);
   
   CodeVector::Word pos()const
   { return m_pos; }

   Type type()const
   { return m_type; }

   CodeVector::Word value()const
   { return m_value; }

   const Deltas& immDeltas()const
   { return m_immDeltas; }

   void link(CodeVector::Word addr, const Deltas& immDeltas);
};

}   //namespace Moove

#endif    //MOOVE_LINKER_REF_HPP
