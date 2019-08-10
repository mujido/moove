#include "moosh_builtins.hpp"

#include <except.hpp>
#include <intvar.hpp>
#include <realvar.hpp>
#include <strvar.hpp>
#include <listvar.hpp>
#include <reply.hpp>

#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace Moove;

Reply builtin_input(ExecutionState& execState, std::auto_ptr<ListVar> args)
{
    MOOVE_ASSERT(args->contents()->size() == 1, "invald number of arguments to builtin 'input'");
    boost::shared_ptr<StrVar> inputType = boost::dynamic_pointer_cast<StrVar>((*args->contents())[0]);
    MOOVE_ASSERT(inputType, "invalid type");

    std::cout << "Enter value: ";

    if(inputType->value() == "int") {
        int value;
            
        MOOVE_ASSERT(std::cin >> value, "unable to input value");
        std::cin.ignore();
        return Reply(Reply::NORMAL, std::auto_ptr<Variant>(execState.intFactory().createValue(value)));
    } else if(inputType->value() == "real") {
        double value;
            
        MOOVE_ASSERT(std::cin >> value, "unable to input value");
        std::cin.ignore();
        return Reply(Reply::NORMAL, std::auto_ptr<Variant>(execState.realFactory().createValue(value)));
    } else if(inputType->value() == "str") {
        std::string value;

        MOOVE_ASSERT(std::getline(std::cin, value), "unable to input value");
        return Reply(Reply::NORMAL, std::auto_ptr<Variant>(execState.strFactory().createValue(value)));
    }

    MOOVE_THROW("invalid input type");
}

namespace {

Reply builtin_print_or_println(ExecutionState& execState, std::auto_ptr<ListVar> args, bool lineFlag)
{
    ListVar::Container::const_iterator argsEnd = args->contents()->end();
    for(ListVar::Container::const_iterator arg = args->contents()->begin(); arg != argsEnd; ++arg) {
        if(boost::dynamic_pointer_cast<IntVar>(*arg) || 
           boost::dynamic_pointer_cast<RealVar>(*arg) || 
           boost::dynamic_pointer_cast<ListVar>(*arg)) 
            std::cout << (*arg)->debugStr();
        else if(boost::shared_ptr<StrVar> strValue = boost::dynamic_pointer_cast<StrVar>(*arg))
            std::cout << strValue->value();
        else
            MOOVE_THROW("invalid type: " + (*arg)->factory().regEntry().name());
    }
     
    if (lineFlag)
        std::cout << std::endl;
    else
        std::cout.flush();

    return Reply();
}

}       // namespace

Reply builtin_print(ExecutionState& execState, std::auto_ptr<ListVar> args)
{
    return builtin_print_or_println(execState, args, false);
}

Reply builtin_println(ExecutionState& execState, std::auto_ptr<ListVar> args)
{
    return builtin_print_or_println(execState, args, true);
}

Reply builtin_length(ExecutionState& execState, std::auto_ptr<ListVar> args)
{
    MOOVE_ASSERT(args->contents()->size() == 1, "invalid number of arguments");
    std::auto_ptr<Variant> result;

    if(boost::shared_ptr<StrVar> strValue = boost::dynamic_pointer_cast<StrVar>((*args->contents())[0]))
        result.reset(execState.intFactory().createValue(strValue->value().size()));
    else if(boost::shared_ptr<ListVar> listValue = boost::dynamic_pointer_cast<ListVar>((*args->contents())[0]))
        result.reset(execState.intFactory().createValue(listValue->contents()->size()));
    else
        MOOVE_THROW("invalid type: " + (*args->contents())[0]->factory().regEntry().name());

    return Reply(Reply::NORMAL, result);
}

Reply builtin_chr(ExecutionState& execState, std::auto_ptr<ListVar> args)
{
    MOOVE_ASSERT(args->contents()->size() == 1, "invalid number of arguments");

    if(boost::shared_ptr<IntVar> intValue = boost::dynamic_pointer_cast<IntVar>((*args->contents())[0])) {
        MOOVE_ASSERT(intValue->value() > 0 && intValue->value() <= 255, "chr() argument out of range");
        return Reply(Reply::NORMAL, std::auto_ptr<Variant>(execState.strFactory().createValue(std::string(1,
                                                                                                          intValue->value()))));
        }

    MOOVE_THROW("invalid type: " + (*args->contents())[0]->factory().regEntry().name());
}

