//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "variant_factory.hpp"

namespace Moove {

Variant* VariantFactory::create(std::unique_ptr<Variant> value)
{
   throw UnexpectedValue("Unexpected value initializer in Moove::VariantFactory::create()");
}

}    //namespace Moove

