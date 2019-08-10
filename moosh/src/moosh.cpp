#include "msg_handler.hpp"
#include "moosh_builtins.hpp"

#include <parser.hpp>
#include <compiler.hpp>
#include <exec_state.hpp>
#include <interpreter.hpp>
#include <default_ops.hpp>
#include <intvar.hpp>
#include <realvar.hpp>
#include <strvar.hpp>
#include <listvar.hpp>

#include <iostream>
#include <fstream>
#include <memory>

using namespace Moove;

typedef DefaultOperatorPackage<DefaultIntVar, DefaultIntVar, IntVar, IntVar, OpcodeInfo::ALL_FUNCTIONAL_GROUPS> IntOpSet;
typedef DefaultOperatorPackage<DefaultRealVar, DefaultIntVar, RealVar, RealVar, OpcodeInfo::ALL_FUNCTIONAL_GROUPS> RealOpSet;
typedef DefaultOperatorPackage<DefaultRealVar, DefaultIntVar, IntVar, RealVar, OpcodeInfo::ALL_FUNCTIONAL_GROUPS> IntRealOpSet;
typedef DefaultOperatorPackage<DefaultRealVar, DefaultIntVar, RealVar, IntVar, OpcodeInfo::ALL_FUNCTIONAL_GROUPS> RealIntOpSet;
typedef DefaultOperatorPackage<DefaultStrVar, DefaultIntVar, StrVar, StrVar, OpcodeInfo::ALL_FUNCTIONAL_GROUPS> StrOpSet;

void registerTypes(ExecutionState& execState)
{
    const TypeRegistry::TypeEntry& intType = execState.typeRegistry().registerType("int", DefaultIntVar::classFactory());
    const TypeRegistry::TypeEntry& realType = execState.typeRegistry().registerType("real", DefaultRealVar::classFactory());
    const TypeRegistry::TypeEntry& strType = execState.typeRegistry().registerType("str", DefaultStrVar::classFactory());
    const TypeRegistry::TypeEntry& listType = execState.typeRegistry().registerType("list", DefaultListVar::classFactory());

    boost::shared_ptr<OperatorPackage> intPkg(new IntOpSet);
    boost::shared_ptr<OperatorPackage> realPkg(new RealOpSet);
    boost::shared_ptr<OperatorPackage> intRealPkg(new IntRealOpSet);
    boost::shared_ptr<OperatorPackage> realIntPkg(new RealIntOpSet);
    boost::shared_ptr<OperatorPackage> strPkg(new StrOpSet);
    
    execState.operatorMap().registerUnary(intType, intPkg);
    execState.operatorMap().registerUnary(realType, realPkg);
    execState.operatorMap().registerUnary(strType, strPkg);

    execState.operatorMap().registerBinary(intType, intType, intPkg);
    execState.operatorMap().registerBinary(realType, realType, realPkg);
    execState.operatorMap().registerBinary(intType, realType, intRealPkg);
    execState.operatorMap().registerBinary(realType, intType, realIntPkg);
    execState.operatorMap().registerBinary(strType, strType, strPkg);
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
