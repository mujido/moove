#include "disassembler.hpp"
#include "opcodes.hpp"

#include <map>
#include <cassert>

namespace Moove {

const char* getImmediateTypeStr(ImmediateValue::Type immType)
{
   switch(immType) {
      case ImmediateValue::INTEGER:
	 return "integer";

      case ImmediateValue::LABEL:
	 return "label";

      case ImmediateValue::LITERAL:
	 return "literal";

      case ImmediateValue::TEMP:
	 return "temp";

      case ImmediateValue::FORK:
	 return "fork";
   }

   return 0;
}

}	//namespace Moove

