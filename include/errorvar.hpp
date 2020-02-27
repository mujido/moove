//Copyright (c) 2005 Kurt Stutsman. All rights reserved.
/**
 * \file
 * \brief Basic Moove::Variant wrapper for representing interpreter errors.
 */
#ifndef MOOVE_ERRORVAR_HPP
#define MOOVE_ERRORVAR_HPP

#include "variant.hpp"
#include "variant_factory.hpp"

namespace Moove {

struct ErrorVar : public Variant {
    struct Factory : public VariantFactory {
        virtual ErrorVar* create(std::unique_ptr<Variant> value) = 0;
    };

    virtual const Variant& value()const = 0;
};

class DefaultErrorVar : public ErrorVar {
public:
    struct Factory : public ErrorVar::Factory {
        DefaultErrorVar* create(std::unique_ptr<Variant> value);
    };

private:
    std::unique_ptr<Variant>      m_value;

public:
#endif
