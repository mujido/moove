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
#include <sstream>

using namespace Moove;
namespace mpl = boost::mpl;

template<typename Left, typename Right>
struct operator_traits;

template<>
struct operator_traits<IntVar, IntVar> {
    typedef DefaultIntVar math_result_type;
    typedef DefaultIntVar comparison_result_type;
};

template<>
struct operator_traits<IntVar, RealVar> {
    typedef DefaultRealVar math_result_type;
    typedef DefaultRealVar comparison_result_type;
};

template<>
struct operator_traits<RealVar, IntVar> : operator_traits<IntVar, RealVar> {};

template<>
struct operator_traits<RealVar, RealVar> {
    typedef DefaultRealVar math_result_type;
    typedef DefaultRealVar comparison_result_type;
};

template<typename ResultVar>
inline Reply makeReply(const typename ResultVar::value_type& value)
{
    return Reply(Reply::NORMAL, boost::shared_ptr<ResultVar>(ResultVar::classFactory().createValue(value)));
}

template<class X, class Y>
DefaultRealVar::value_type mooshMod(X x, Y y)
{
    return std::fmod(static_cast<DefaultRealVar::value_type>(x),
                     static_cast<DefaultRealVar::value_type>(y));
}

template<class X, class N>
DefaultRealVar::value_type mooshPow(X x, N n)
{
    return std::pow(static_cast<DefaultRealVar::value_type>(x),
                    static_cast<DefaultRealVar::value_type>(n));
}

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
            if((n & 1) != 0) {
                result *= x;
            }

            x *= x;
            n >>= 1;
        }
    }

    return result;
}

template<class LeftVariant, class RightVariant>
Reply mooshBinaryOpDispatch(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)
{
    typedef typename operator_traits<LeftVariant, RightVariant>::math_result_type MathResultVar;
    typedef typename operator_traits<LeftVariant, RightVariant>::comparison_result_type CmpResultVar;

    typename LeftVariant::value_type leftVal = static_cast<const LeftVariant&>(*leftVar).value();
    typename RightVariant::value_type rightVal = static_cast<const RightVariant&>(*rightVar).value();

    Reply reply;

    switch(op) {
        case OP_ADD:
            reply = makeReply<MathResultVar>(leftVal + rightVal);
            break;

        case OP_SUB:
            reply = makeReply<MathResultVar>(leftVal - rightVal);
            break;

        case OP_MUL:
            reply = makeReply<MathResultVar>(leftVal * rightVal);
            break;

        case OP_DIV:
            reply = makeReply<MathResultVar>(leftVal / rightVal);
            break;

        case OP_MOD:
            reply = makeReply<MathResultVar>(mooshMod(leftVal, rightVal));
            break;

        case OP_EXP:
            reply = makeReply<MathResultVar>(mooshPow(leftVal, rightVal));
            break;

        case OP_EQ:
            reply = makeReply<CmpResultVar>(leftVal == rightVal);
            break;

        case OP_NE:
            reply = makeReply<CmpResultVar>(leftVal != rightVal);
            break;

        case OP_LT:
            reply = makeReply<CmpResultVar>(leftVal < rightVal);
            break;

        case OP_LE:
            reply = makeReply<CmpResultVar>(leftVal <= rightVal);
            break;

        case OP_GE:
            reply = makeReply<CmpResultVar>(leftVal >= rightVal);
            break;

        case OP_GT:
            reply = makeReply<CmpResultVar>(leftVal > rightVal);
            break;

        default:
            MOOVE_THROW("Unrecognized opcode");
    }

    return reply;
}

Reply mooshStringOpDispatch(Opcode op, std::unique_ptr<Variant> leftVar, std::unique_ptr<Variant> rightVar)
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
    execState.typeRegistry().registerType("list", DefaultListVar::classFactory());

    //execState.operatorMap().registerUnary(intType, &mooshBinaryOpDispatch<DefaultIntVar, IntVar, IntVar>);
    //execState.operatorMap().registerUnary(realType, &mooshBinaryOpDispatch<DefaultRealVar, RealVar, RealVar>);
    //execState.operatorMap().registerUnary(strType, &mooshStringOpDispatch);

    execState.operatorMap().registerBinary(intType, intType, &mooshBinaryOpDispatch<IntVar, IntVar>);
    execState.operatorMap().registerBinary(realType, realType, &mooshBinaryOpDispatch<RealVar, RealVar>);
    execState.operatorMap().registerBinary(intType, realType, &mooshBinaryOpDispatch<IntVar, RealVar>);
    execState.operatorMap().registerBinary(realType, intType, &mooshBinaryOpDispatch<RealVar, IntVar>);
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

std::unique_ptr<Variant> evalScript(ExecutionState& execState,
                                  const std::string& script,
                                  ListVar::Container& args,
                                  MessageHandler& msgs,
                                  bool traceFlag = false)
{
    std::unique_ptr<Variant> returnValue;

    Parser parser;
    if(parser.parse(script, msgs, false)) {
        std::unique_ptr<Program> program = parser.releaseProgram();

        Compiler compiler(execState.typeRegistry());

        std::unique_ptr<DebugBytecodeProgram> bc = compiler.compileDebug(*program);

        Interpreter::VariableDefMap varDefs;
        std::string key("args");

        varDefs.insert(key, DefaultListVar::classFactory().createList(args));
        returnValue = execState.call(*bc, varDefs, traceFlag);
    }

    return returnValue;
}

std::unique_ptr<Variant> evalExpr(ExecutionState& execState,
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
        std::unique_ptr<Variant> result = evalExpr(execState, args[i], msgs, traceFlag);

        if (!result.get()) {
            std::cerr << "Invalid argument: " << args[i] << std::endl;
            exit(1);
        }

        parsedList.push_back(boost::shared_ptr<Variant>(result.release()));
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

        std::ostringstream inBuf;
        inBuf << inStream->rdbuf();
        source = inBuf.str();

        if(source.length() > 2 && source[0] == '#' && source[1] == '!') {
            // ignore executable line. Increase lineOffset to keep line numbers correct
            ++lineOffset;
            auto lineEnd = std::find(source.begin() + 2, source.end(), '\n');
            if (lineEnd != source.end())
                source.erase(source.begin(), lineEnd + 1);
        }

        // clear EOF state so that executing code can input from stdin
        std::cin.clear();

        // attempt to parse arguments and store into a list
        ListVar::Container argContents;
        if(scriptArgsIndex != -1) {
            parseScriptArgs(execState, &argv[scriptArgsIndex], argc - scriptArgsIndex, argContents, traceStack);
        }

        MessageHandler msgs(0);
        std::unique_ptr<Variant> result = evalScript(execState, source, argContents, msgs, traceStack);
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
