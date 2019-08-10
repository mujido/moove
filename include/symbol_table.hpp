//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_SYMBOL_TABLE_HPP
#define MOOVE_SYMBOL_TABLE_HPP

#include "string_utils.hpp"

#include <set>
#include <string>
#include <utility>
#include <boost/shared_ptr.hpp>

namespace Moove {

class Symbol {
private:
   const std::string* m_name;

public:
   Symbol() : m_name(0)
   {}

   explicit Symbol(const std::string* name) : m_name(name)
   {}

   const std::string& name()const;
      
   operator bool ()const
   { return m_name != 0; }

   bool operator == (const Symbol& sym)const
   { return m_name == sym.m_name; }

   bool operator != (const Symbol& sym)const
   { return m_name != sym.m_name; }

   bool operator < (const Symbol& sym)const;
};

class SymbolTable {
private:
   typedef std::set<std::string, iless> Table;

   /**
    * \internal
    * Holds the table of string symbols
    * 
    * Uses a boost::shared_ptr<> so that symbol tables can be copied back and forth while keeping
    * all Symbol() references valid for any table.
    */
   boost::shared_ptr<Table> m_table;

public:
   typedef Table::const_iterator const_iterator;
   typedef const_iterator        iterator;
   typedef  Table::size_type     size_type;

public:
   SymbolTable();

   size_type size()const
   { return m_table->size(); }

   const_iterator begin()const
   { return m_table->begin(); }

   const_iterator end()const
   { return m_table->end(); }
   
   Symbol findSymbol(const std::string& name)const;

   std::pair<Symbol, bool> insert(const std::string& name);

   void erase(Symbol sym);

   void erase(const std::string& name);

   void erase(iterator pos)
   { m_table->erase(pos); }
};

}   //namespace Moove

#endif   //MOOVE_SYMBOL_TABLE_HPP
