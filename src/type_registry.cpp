//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "type_registry.hpp"
#include "variant_factory.hpp"

namespace Moove {

bool TypeRegistry::TypeEntryNameLessThan::operator () (const TypeEntry& x, const TypeEntry& y)const
{
   return x.name() < y.name();
}

const TypeRegistry::TypeEntry* TypeRegistry::findType(const std::string& name)const
{
   Registry::const_iterator pos = m_registry.find(TypeEntry(name));
   
   if(pos != m_registry.end())
      return &*pos;
   else
      return 0;
}

const TypeRegistry::TypeEntry* TypeRegistry::findType(TypeID id)const
{
   // if id is less than m_enumerator.size(), assume it's a valid TypeID
   if(id < m_enumerator.size())
      return m_enumerator[id];

   // id is invalid so return null
   return 0;
}

const TypeRegistry::TypeEntry&
TypeRegistry::registerType(const std::string& name, VariantFactory& factory, bool replace)
{
   TypeEntry tmpEntry(name, m_enumerator.size(), factory);

   // first check if this type is already registered
   Registry::iterator pos = m_registry.find(tmpEntry);

   if(pos != m_registry.end()) {
      // the type does exist. OK if replace == true. Otherwise, throw exception
      if(!replace)
	 throw DuplicateType("TypeRegistry", "registerType", "duplicate type encountered");
      else
	 m_registry.erase(pos);
   }
    
   // add factory to registry and enumerator
   pos = m_registry.insert(tmpEntry).first;
   m_enumerator.push_back(&*pos);
   factory.setRegEntry(*pos);

   return *pos;
}

}   //namespace Moove
