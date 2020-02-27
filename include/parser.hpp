//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file parser.hpp
 * \brief Main header file for MOOVE source parsing
 */
#ifndef MOOVE_PARSER_HPP
#define MOOVE_PARSER_HPP

#include "parser_state.hpp"
#include "program_ast.hpp"

#include <bitset>
#include <memory>
#include <stdexcept>

namespace Moove {

class ParserMessages;

/**
 * \brief Converts MOO source code into AST form
 *
 * This class represents the interface between client code and the internal
 * parsing funtionality. An instance of this class allows one to parse
 * source into an AST representation.
 *
 * In multithreaded applications, it also ensures that the non-reentrant
 * parser generated by yacc or similar clones is properly managed (locked and
 * unlocked) as various threads may attempt to use it.
 */
class Parser {
private:      
   std::unique_ptr<ParserState> m_state;

public:
#if 0
   /**
    * \brief Parsing options
    *
    * These options should not be OR'd together. Instead, they represent
    * the index of the bit to set in a Options bitset container.
    *
    * All options represent the opposite of the default. For instance,
    * DISABLE_OBJNUMS overrides the default behavior of allowing objnums
    * to appear in the source.
    */
   enum {
      DISABLE_SWITCH,              ///< Disable switch syntax
      DISABLE_OBJNUMS,             ///< Disable object numbers
      DISABLE_INC_DEC,             ///< Disable increment/decrement operators
      DISABLE_ASSIGN_OPS,          ///< Disable extended (+=) assign. operators
      NUM_OPTIONS
   };

   ///Holds parsing options in a std::bitset format
   typedef std::bitset<NUM_OPTIONS> Options;
#endif

   /**
    * \brief Thrown if an attempt to release a erroneous program occurs
    *
    * Thrown by releaseProgram() when hasErrors() is true and hence no 
    * program could correctly be parsed.
    */
   typedef std::logic_error MisformedProgram;

   ///Create a parser object
   Parser()
   {}

   /**
    * \brief Create a parser object and parse source
    * \param source A string of Moove code to parse
    * \param msgs A message callback object for errors and warnings
    * \param opts An options bitset for options to parse with
    */
   Parser(const char* source, ParserMessages& msgs, bool objnums);

   /**
    * \brief Create a parser object and attempt to parse source
    * \param source Source code to parse
    * \param opts Set of parsing options
    *
    * Although the boolean success code will not be returned, if errors()
    * is not empty, compilation was not successful.
    */
   explicit Parser(const std::string& source, 
		   ParserMessages& msgs, 
		   bool objnums);

   bool parse(const char* source, ParserMessages& msgs, bool objnums);

   /**
    * \brief Attempt to parse source code
    * \param source Source code to parse
    * \param opts Set of parsing options
    * \return If the parse was successful, \c true. Otherwise, \c false
    *
    * If errors were encountered (indicated by \c false return value), errors()
    * can be used to discover them. If warnings were generated, the parsing
    * will still be successful (\c true return value) but warnings() will
    * contain the messages.
    */
   bool parse(const std::string& source, ParserMessages& msgs, bool objnums);

   bool hasErrors()const;

   std::unique_ptr<Program> releaseProgram();
};

}   //namespace Moove

#endif    //MOOVE_PARSER_HPP

