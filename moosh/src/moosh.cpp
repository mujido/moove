#include "msg_handler.hpp"
#include "moosh_builtins.hpp"

#include <parser.hpp>
#include <compiler.hpp>
#include <exec_state.hpp>
#include <interpreter.hpp>
// #include <default_ops.hpp>
#include <intvar.hpp>
#include <realvar.hpp>
#include <strvar.hpp>
#include <listvar.hpp>
#include "op_wrappers.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

using namespace Moove;

template<class X, class Y>
DefaultRealVar::value_type mooshMod(X x, Y y)
{
    return std::fmod(static_cast<DefaultRealVar::value_type>(x), 
                     static_cast<DefaultRealVar::value_type>(n));
}

template<class X, class N>
DefaultRealVar::value_type mooshPow(X x, N n)
{
    return std::pow(static_cast<DefaultRealVar::value_type>(x), 
                    static_cast<DefaultRealVar::value_type>(n));
}

template<>
DefaultIntVar::value_type mooshPow(DefaultIntVar::value_type x, DefaultIntVar::value_type n)
{
    DefaultIntVar::value_type result = 1;

    if (n < 0) {
        switch(x) {
            case -1:
                result = (n % 2 == 0) ? 1 : -1;
                break;

            case 0:
                result = 0;       // this should be an error (E_DIV)
                break;

            case 1:
                result = 1;
                break;

            default:
                result = 0;
                break;
        }
    } else {
        while(n > 0) {
            if(n & 1 != 0) {
                result *= x;
            }

            x *= x;
            n >>= 1;
        }
    }

    return result;
}

template<class ResultVariant, class LeftVariant, class RightVariant>
Reply mooshBinaryMathDispatch(Opcode op, std::auto_ptr<Variant> leftVar, std::auto_ptr<Variant> rightVar)
{
    typename ResultVariant::value_type value;

    typename LeftVariant::value_type leftVal = static_cast<const LeftVariant&>(*leftVar).value();
    typename RightVariant::value_type rightVal = static_cast<const RightVariant&>(*rightVar).value();

    switch(op) {
        case OP_ADD:
            value = leftVal + rightVal;
            break;

        case OP_SUB:
            value = leftVal - rightVal;
            break;

        case OP_MUL:
            value = leftVal * rightVal;
            break;

        case OP_DIV:
            value = leftVal / rightVal;
            break;

        case OP_MOD:
            value = mooshMod(leftVal, rightVal);
            break;

        case OP_EXP:
            value = mooshPow(leftVal, rightVal);
            break;

        default:
            MOOVE_THROW("Unrecognized opcode");
    }

    boost::shared_ptr<ResultVariant> resultVar(static_cast<ResultVariant*>(ResultVariant::classFactory().create()));
    return Reply(Reply::NORMAL, resultVar);
}
            
template<class LeftVariant, class RightVariant>
Reply mooshBinaryComparisonDispatch(Opcode op, std::auto_ptr<Variant> leftVar, std::auto_ptr<Variant> rightVar)
{
    switch(op) {
        case OP_EQ:
            value = leftVal == rightVal;
            break;

        case OP_NE:
            value = leftVal != rightVal;
            break;

        case OP_LT:
            value = leftVal < rightVal;
            break;

        case OP_LE:
            value = leftVal <= rightVal;
            break;

        case OP_GE:
            value = leftVal >= rightVal;
            break;

        case OP_GT:
            value = leftVal > rightVal;
            break;

        default:
            MOOVE_THROW("Unrecognized opcode");
    }
    
template<class ResultVariant, class LeftVariant, class RightVariant>
Reply mooshBinaryOpDispatch(Opcode op, std::auto_ptr<Variant> leftVar, std::auto_ptr<Variant> rightVar)
{
    typename ResultVariant::value_type value;

    typename LeftVariant::value_type leftVal = static_cast<const LeftVariant&>(*leftVar).value();
    typename RightVariant::value_type rightVal = static_cast<const RightVariant&>(*rightVar).value();

    switch(op) {
        case OP_ADD:
            value = leftVal + rightVal;
            break;

        case OP_SUB:
            value = leftVal - rightVal;
            break;

        case OP_MUL:
            value = leftVal * rightVal;
            break;

        case OP_DIV:
            value = leftVal / rightVal;
            break;

        case OP_MOD:
            value = std::fmod(static_cast<DefaultRealVar::value_type>(leftVal),
                              static_cast<DefaultRealVar::value_type>(rightVal));
            break;

        case OP_EXP:
            value = std::pow(static_cast<DefaultRealVar::value_type>(leftVal),
                             static_cast<DefaultRealVar::value_type>(rightVal));
            break;


        default:
    }

    boost::shared_ptr<ResultVariant> resultVar(static_cast<ResultVariant*>(ResultVariant::classFactory().create()));
    return Reply(Reply::NORMAL, resultVar);
}

Reply mooshStringOpDispatch(Opcode op, std::auto_ptr<Variant> leftVar, std::auto_ptr<Variant> rightVar)
{
    const StrVar::value_type& leftVal = static_cast<const StrVar&>(*leftVar).value();
    const StrVar::value_type& rightVal = static_cast<const StrVar&>(*rightVar).value();
    
    if (op != OP_ADD)
        MOOVE_THROW("Unrecognized opcode");

    boost::shared_ptr<DefaultStrVar> resultVar(static_cast<DefaultStrVar*>(DefaultStrVar::classFactory().create()));
    resultVar->setValue(leftVal + rightVal);
    return Reply(Reply::NORMAL, resultVar);
}
    

void registerTypes(ExecutionState& execState)
{
    const TypeRegistry::TypeEntry& intType = execState.typeRegistry().registerType("int", DefaultIntVar::classFactory());
    const TypeRegistry::TypeEntry& realType = execState.typeRegistry().registerType("real", DefaultRealVar::classFactory());
    const TypeRegistry::TypeEntry& strType = execState.typeRegistry().registerType("str", DefaultStrVar::classFactory());
    const TypeRegistry::TypeEntry& listType = execState.typeRegistry().registerType("list", DefaultListVar::classFactory());

    //execState.operatorMap().registerUnary(intType, &mooshBinaryOpDispatch<DefaultIntVar, IntVar, IntVar>);
    //execState.operatorMap().registerUnary(realType, &mooshBinaryOpDispatch<DefaultRealVar, RealVar, RealVar>);
    //execState.operatorMap().registerUnary(strType, &mooshStringOpDispatch);

    execState.operatorMap().registerBinary(intType, intType, &mooshBinaryOpDispatch<DefaultIntVar, IntVar, IntVar>);
    execState.operatorMap().registerBinary(realType, realType, &mooshBinaryOpDispatch<DefaultRealVar, RealVar, RealVar>);
    execState.operatorMap().registerBinary(intType, realType, &mooshBinaryOpDispatch<DefaultRealVar, IntVar, RealVar>);
    execState.operatorMap().registerBinary(realType, intType, &mooshBinaryOpDispatch<DefaultRealVar, RealVar, IntVar>);
    execState.operatorMap().registerBinary(strType, strType, &mooshStringOpDispatch);
}

void registerBuiltins(ExecutionState& execState)
{
    execState.builtinRegistry().registerFunction("input", &builtin_input);
    execState.builtinRegistry().registerFunction("print", &builtin_print);
    execState.builtinRegistry().registerFunction("println", &builtin_println);
    execState.builtinRegistry().registerFunction("length", &builtin_length);
    execState.builtinRegistry().registerFunction("chr", &builtin_chr);
}

std::auto_ptr<Variant> evalScript(ExecutionState& execState, 
                                  const std::string& script, 
                                  ListVar::Container& args, 
                                  MessageHandler& msgs,
                                  bool traceFlag = false)
{
    std::auto_ptr<Variant> returnValue;

    Parser parser;
    if(parser.parse(script, msgs, false)) {
        std::auto_ptr<Program> program = parser.releaseProgram();

        Compiler compiler(execState.typeRegistry());
        
        std::auto_ptr<DebugBytecodeProgram> bc = compiler.compileDebug(*program);
        
        Interpreter::VariableDefMap varDefs;
        std::string key("args");

        varDefs.insert(key, DefaultListVar::classFactory().createList(args));
        returnValue = execState.call(*bc, varDefs, traceFlag);
    }

    return returnValue;
}

std::auto_ptr<Variant> evalExpr(ExecutionState& execState, 
                                const std::string& valueStr, 
                                MessageHandler& msgs, 
                                bool traceFlag = false)
{
    ListVar::Container args;

    return evalScript(execState, "return " + valueStr + ";", args, msgs, traceFlag);
}

void parseScriptArgs(ExecutionState& execState, 
                     char **args, 
                     int argCount, 
                     ListVar::Container& parsedList,
                     bool traceFlag = false)
{
    MessageHandler msgs(0);

    for(int i = 0; i < argCount; ++i) {
        std::auto_ptr<Variant> result = evalExpr(execState, args[i], msgs, traceFlag);

        if (!result.get()) {
            std::cerr << "Invalid argument: " << args[i] << std::endl;
            exit(1);
        }

        parsedList.push_back(boost::shared_ptr<Variant>(result));
    }
}

int main(int argc, char **argv)
{
    try {
        std::string source, line;
        std::ifstream file;
        std::istream* inStream = &std::cin;
        unsigned lineOffset = 0;
        int scriptArgsIndex = -1;
        bool traceStack = false;
        
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "--") == 0) {
                scriptArgsIndex = i + 1;
                break;
            } else if(strcmp(argv[i], "-t") == 0) {
                traceStack = true;
            } else if(inStream == &std::cin) {
                file.open(argv[i], std::ios::in);
                if(!file) {
                    std::cerr << "Error opening file '" << argv[i] << "'\n";
                    return 1;
                }

                inStream = &file;
            } else {
                scriptArgsIndex = i;
                break;
            }
        }
                
        // create a new execution state object
        ExecutionState execState;

        registerTypes(execState);
        registerBuiltins(execState);

        while(std::getline(*inStream, line)) {
            if(source.empty() && line.length() > 2 && line[0] == '#' && line[1] == '!') {
                // ignore executable line. Increase lineOffset to keep line numbers correct
                ++lineOffset;
                continue;
            }

            source += line + '\n';
        }

        // clear EOF state so that executing code can input from stdin
        std::cin.clear();

        // attempt to parse arguments and store into a list
        ListVar::Container argContents;
        if(scriptArgsIndex != -1) {
            parseScriptArgs(execState, &argv[scriptArgsIndex], argc - scriptArgsIndex, argContents, traceStack);
        }
        
        MessageHandler msgs(0);
        std::auto_ptr<Variant> result = evalScript(execState, source, argContents, msgs, traceStack);
        if(result.get()) {
            std::cout << "Result: " << result->debugStr() << "\n";
        } else {
            return 1;
        }
    } catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
