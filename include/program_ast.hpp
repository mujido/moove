//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_PROGRAM_AST_HPP
#define MOOVE_PROGRAM_AST_HPP

#include "stmt_ast.hpp"
#include "symbol_table.hpp"

#include <memory>
#include <boost/utility.hpp>

namespace Moove {

/**
 * \brief Contains an AST program tree
 *
 * Stores a parsed program in AST format as a collection of Stmt::Stmt
 * objects. The SymbolTable is also stores within this class so that the
 * Symbol values found in the AST objects can be used.
 */
class Program : boost::noncopyable {
private:
   std::unique_ptr<Stmt::Block> m_stmts;
   SymbolTable                m_varTable;

public:
   /**
    * \brief Retrieve the collection of statements found in the program
    * \return Stmt::Block object containing the Stmt::Stmt objects
    */
   const Stmt::Block& stmts()const
   { return *m_stmts; }

   /**
    * \brief Retrieve the symbol table used for variables
    * \return SymbolTable object associated with the Symbol values found in
    *         the AST.
    */   
   const SymbolTable& varTable()const
   { return m_varTable; }

   /**
    * \internal
    * \brief Retrieve the symbol table used for variables
    * \return SymbolTable object associated with the Symbol values found in
    *         the AST.
    */   
   SymbolTable& varTable()
   { return m_varTable; }

   /**
    * \internal
    * \brief Assign the statements for this program
    * \param stmts Statements used in the program
    */
   void setStmts(std::unique_ptr<Stmt::Block> stmts)
   { m_stmts = std::move(stmts); }
};

}   //namespace Moove

#endif   //MOOVE_PROGRAM_AST_HPP

