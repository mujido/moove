#include "symbol_table.hpp"

#include <cassert>

namespace Moove {

const std::string& Symbol::name()const
{
   assert(m_name != 0);
   return *m_name;
}

bool Symbol::operator < (const Symbol& sym)const
{
   assert(m_name != 0 && sym.m_name != 0);
   return iless()(*m_name, *sym.m_name);
}

SymbolTable::SymbolTable() : m_table(new Table)
{}

Symbol SymbolTable::findSymbol(const std::string& name)const
{
   const_iterator pos = m_table->find(name);
   if(pos != m_table->end())
      return Symbol(&*pos);
   else
      return Symbol();
}

std::pair<Symbol, bool> SymbolTable::insert(const std::string& name)
{
   bool added = false;
   const_iterator pos = m_table->find(name);
   if(pos == m_table->end()) {
      pos = m_table->insert(name).first;
      added = true;
   }
   
   return std::pair<Symbol, bool>(Symbol(&*pos), added);
}

void SymbolTable::erase(Symbol sym)
{
   iterator pos = m_table->find(sym.name());
   if(pos != m_table->end())
      m_table->erase(pos);
}

void SymbolTable::erase(const std::string& name)
{
   iterator pos = m_table->find(name);
   if(pos != m_table->end())
      m_table->erase(pos);
}

}	//namespace Moove
