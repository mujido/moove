#include "listvar.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace Moove {

DefaultListVar::Factory DefaultListVar::s_factory;

std::string ListVar::debugStr()const
{
    std::ostringstream str;

    str << '{';

    Container::const_iterator elem = contents()->begin();
    Container::const_iterator end = contents()->end();

    if (elem != end) {
        str << (*elem)->debugStr();

        for(++elem; elem != end; ++elem)
            str << ", " << (*elem)->debugStr();
    }

    str << '}';
    return str.str();
}
      
DefaultListVar* DefaultListVar::Factory::create()
{
    return new DefaultListVar;
}

DefaultListVar* DefaultListVar::Factory::createList(Container& cont)
{
    std::unique_ptr<DefaultListVar> var(new DefaultListVar);
    var->setContents(cont);
    return var.release();
}

DefaultListVar* DefaultListVar::Factory::createListCopy(const ListVar& var)
{
    std::unique_ptr<DefaultListVar> newVar(new DefaultListVar);
    newVar->copyContents(var.contents());
    return newVar.release();
}

DefaultListVar::Container::value_type DefaultListVar::cloneElement(Container::value_type elem)
{
    return Container::value_type(elem->clone());
}

DefaultListVar::DefaultListVar(const ListVar& var)
{
    copyContents(var.contents());
}

DefaultListVar* DefaultListVar::clone()const
{
    return s_factory.createListCopy(*this);
}

void DefaultListVar::setContents(Container& cont)
{
    m_contents.reset(new Container(cont));
}

void DefaultListVar::copyContents(ContainerRef cont)
{
    m_contents = cont;
}

void DefaultListVar::setIndex(Container::size_type index, boost::shared_ptr<Variant> var)
{
    MOOVE_ASSERT(index < m_contents->size(), "index out of range");
    
    // clone shared pointer if not sole owner
    if (!m_contents.unique()) {
        m_contents.reset(new Container(*m_contents));
    }

    (*m_contents)[index] = var;
}

void DefaultListVar::appendValue(boost::shared_ptr<Variant> var)
{
    // clone shared pointer if not sole owner
    if (!m_contents.unique()) {
        m_contents.reset(new Container(*m_contents));
    }

    m_contents->push_back(var);
}

DefaultListVar& DefaultListVar::operator = (const ListVar& var)
{
    if (this != &var)
        copyContents(var.contents());

    return *this;
}

}       //namespace Moove
