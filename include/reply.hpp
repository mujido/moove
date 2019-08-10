//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Provides Moove::Variant valued returns with "status codes"
 */
#ifndef MOOVE_REPLY_HPP
#define MOOVE_REPLY_HPP

#include "except.hpp"
#include "variant.hpp"

#include <boost/shared_ptr.hpp>

namespace Moove {

/**
 * \brief Variant valued reply type with status codes
 *
 * This class provides a mechanism for conveying return values and information
 * between functions. In doing so, it also allows for classification of the 
 * variant values returned. This is accomplished by a return type code--a 
 * value of Reply::Type.
 */
class Reply {
public:
   ///All possible states that can be conveyed with the Reply class
   enum Type {
      NORMAL,			///< Normal value, no special processing
      EXCEPTION			///< Interpretation-level exception
   };

private:
   Type				m_type;
   boost::shared_ptr<Variant>	m_value;

public:
   /**
    * \brief Create a default Reply object
    *
    * The type will be set to Reply::NORMAL and the value to NULL.
    */
   Reply() : m_type(NORMAL)
   {}

   /**
    * \brief Create a Reply object with type and value
    * \param type Type of reply
    * \param value Variant object
    */
   Reply(Type type, boost::shared_ptr<Variant> value);

   /**
    * \brief Create a Reply object with type and value
    * \param type Type of reply
    * \param value Variant object
    */
   Reply(Type type, std::auto_ptr<Variant> value);

   /**
    * \brief Retrieve type of reply
    * \return Type of reply
    */
   Type type()const
   { return m_type; }

   /**
    * \brief Retrieve value pointer
    * \return Value object pointer currently assigned to this object
    */
   const boost::shared_ptr<Variant> value()const
   { return m_value; }

   /**
    * \brief Reset to a new reply value
    * \param type Type of reply
    * \param value Pointer to value object assigned to this object
    */
   void reset(Type type, boost::shared_ptr<Variant> value);

   /**
    * \brief Reset to a new reply value
    * \param type Type of reply
    * \param value Pointer to value object assigned to this object
    */
   void reset(Type type, std::auto_ptr<Variant> value);

   /**
    * \brief Check if this Reply object is a normal return value
    * \return Result of comparison <tt>type() == Reply::NORMAL</tt>
    */
   bool normal()const
   { return type() == NORMAL; }

   /**
    * \brief Check if this Reply object is an exception
    * \return Result of comparison <tt>type() == Reply::EXCEPTION</tt>
    */
   bool exception()const
   { return type() == EXCEPTION; }
};
      
}    //namespace Moove

#endif    //MOOVE_REPLY_HPP
