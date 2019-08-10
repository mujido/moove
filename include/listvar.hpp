//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_LISTVAR_HPP
#define MOOVE_LISTVAR_HPP

#include "variant.hpp"
#include "variant_factory.hpp"

#include <vector>
#include <cstddef>
#include <memory>
#include <iterator>
#include <boost/shared_ptr.hpp>

namespace Moove {

struct ListVar : public Variant {
    typedef std::vector<boost::shared_ptr<Variant> > Container;
    typedef boost::shared_ptr<Container>             ContainerRef;

    struct Factory : public VariantFactory {
        /**
         * \brief Create a ListVar and fill with contents of \a cont
         * \param cont Container of values to transfer
         * \return Instance of a descendent of ListVar
         *
         * The contents of \a cont are copied to the new ListVar object.
         */
        virtual ListVar* createList(Container& cont) = 0;

        /**
         * \brief Create a ListVar and fill with copies of the contents of \a cont
         * \param cont Container of values to copy
         * \return Instance of a descendent of ListVar
         *
         * Each object in \a cont will have its Variant::clone() method called and the result will be
         * added to the new ListVar instance.
         */
        virtual ListVar* createListCopy(const ListVar& var) = 0;
    };

    bool truthValue()const
    { return !contents()->empty(); }

    std::string debugStr()const;

    virtual ContainerRef contents()const = 0;

    virtual void setContents(Container& cont) = 0;

    virtual void copyContents(ContainerRef cont) = 0;

    virtual void setIndex(Container::size_type index, boost::shared_ptr<Variant> var) = 0;

    virtual void appendValue(boost::shared_ptr<Variant> var) = 0;
};
  
class DefaultListVar : public ListVar {
public:
    struct Factory : public ListVar::Factory {
        DefaultListVar* create();

        DefaultListVar* createList(Container& cont);

        DefaultListVar* createListCopy(const ListVar& var);
    };

private:
    ContainerRef m_contents;

    static Factory s_factory;

    static Container::value_type cloneElement(Container::value_type elem);

public:
    DefaultListVar() : m_contents(new Container)
    {}

    DefaultListVar(const ListVar& var);

    DefaultListVar* clone()const;

    Factory& factory()const
    { return s_factory; }

    ContainerRef contents()const
    { return m_contents; }

    void setContents(Container& cont);

    void copyContents(ContainerRef  cont);

    void setIndex(Container::size_type index, boost::shared_ptr<Variant> var);

    void appendValue(boost::shared_ptr<Variant> var);

    DefaultListVar& operator = (const ListVar& var);

    static Factory& classFactory()
    { return s_factory; }
};

}       // namespace Moove

#if 0
/**
 * Convert a reference to a value to a pointer
 *
 * Useful with ListVar::setContents() for a container of some Variant type since the method
 * expects a container of pointers to Variant.
 */
template<class T>
struct PointerTo {
    const T* operator () (const T& value)const
    { return &value; }
};

class ListVar : public Variant {
protected:
    virtual void clear() = 0;

    virtual void append(std::auto_ptr<Variant> var);

public:
    typedef size_t size_type;

    class const_iterator {
    private:
        const ListVar*  m_listVar;
        size_type               m_index;

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const Variant*                  value_type;
        typedef std::ptrdiff_t                  difference_type;
        typedef const Variant**                 pointer;
        typedef const Variant*&                 reference;

        explicit const_iterator(const ListVar* listVar, size_type index = 0) : m_listVar(listVar), m_index(index)
        {}

        Variant& operator * ()const
        { return *m_listVar->at(m_index); }

        Variant* operator -> ()const
        { return *m_listVar->at(m_index); }

        const_iterator& operator ++ ()
        {
            ++m_index;
            return *this;
        }

        const_iterator& operator -- ()
        {
            --m_index;
            return *this;
        }

        const_iterator operator ++ (int)
        {
            const_iterator tmp(*this);
            ++*this;
            return tmp;
        }

        const_iterator operator -- (int)
        {
            const_iterator tmp(*this);
            --*this;
            return tmp;
        }

        const_iterator& operator += (difference_type n)
        {
            m_index += n;
            return *this;
        }

        const_iterator opeator + (difference_type n)
        { return const_iterator(m_list, m_index + n); }

        const_iterator& operator -= (difference_type n)
        {
            m_index -= n;
            return *this;
        }

        const_iterator operator - (difference_type n)
        { return const_iterator(m_list, m_index - n); }

        difference_type operator - (const_iterator iter)
        { return static_cast<difference_type>(m_index) - iter.m_index; }
         
        const Variant& operator [] (difference_type index)
        { return *m_listVar->at(m_index + index); }
    };
      
   
   
    virtual size_type size()const = 0;

    virtual const Variant& at(size_type index)const;

    template<class InputIterator>
    void setContents(InputIterator start, InputIterator end)
    {
        clear();
      
        for(; start != end; ++start)
            append((*start)->clone());
    }

    template<class InputIterator, class Converter>
    void setContents(InputIterator start, InputIterator end, Converter conv)
    {
        clear();
      
        for(; start != end; ++start)
            append(conv(*start)->clone();
                   }
    };
   
#endif

#endif  // MOOVE_LISTVAR_HPP
