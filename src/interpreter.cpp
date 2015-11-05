#include "interpreter.hpp"
#include "reply.hpp"
#include "except.hpp"
#include "exec_state.hpp"
#include "opcodes.hpp"
#include "op_map.hpp"
#include "op_package.hpp"
#include "type_registry.hpp"
#include "listvar.hpp"
#include "strvar.hpp"
#include "intvar.hpp"
#include "realvar.hpp"

#include <iostream>

namespace Moove {

void Interpreter::clearTemps()
{
    TemporaryValues::iterator end = m_temps.end();
    for(TemporaryValues::iterator temp = m_temps.begin(); temp != end; ++temp)
        m_temps.replace(temp, 0);
}
   
void Interpreter::defineVariables(VariableDefMap& varDefs)
{
    MOOVE_ASSERT(m_bcDebug, "cannot set variables in a non-debug compilation");

    // make sure that the variable "args" has a value even if not defined in varDefs
    std::string argsStr("args");
    if (varDefs.find(argsStr) == varDefs.end()) {
        ListVar::Container contents;
        varDefs.insert(argsStr, m_execState->listFactory().createList(contents));
    }

    VariableDefMap::iterator endDef = varDefs.end();
    for(VariableDefMap::iterator def = varDefs.begin(); def != endDef; ++def) {
        // if this variable exists (has a defined symbol), set the value
        Symbol varSym = m_bcDebug->varSymTable().findSymbol(def.key());
        if(varSym) {
            CodeVector::Word tempID = m_bcDebug->varIDBySymbol(varSym);
            m_temps.replace(m_temps.begin() + tempID, varDefs.release(def).release());
        }
    }

}

bool Interpreter::execFinished()const
{
    return m_retVal.get() != 0;
}

void Interpreter::execBuiltin()
{
    std::auto_ptr<ListVar> args(popStack<ListVar>());
    std::auto_ptr<StrVar> name(popStack<StrVar>());

    Reply reply;
    BuiltinRegistry::Function builtinFunc = m_execState->builtinRegistry().findFunction(name->value());
    if(!builtinFunc) {
        if(name->value() == "self") {
            VariableDefMap recurseVarDefs;
            std::string argsName("args");

            recurseVarDefs.insert(argsName, args.release());
            reply.reset(Reply::NORMAL, m_execState->call(*m_bcDebug, recurseVarDefs, m_traceFlag));
        } else
            MOOVE_THROW("invalid builtin function: " + name->value());
    } else
        reply = builtinFunc(*m_execState, args);

    // if at this point, a builtin was executed that does not return a value. return a bogus value
    m_stack.push_back(reply.value() ? reply.value()->clone() : m_execState->intFactory().createValue(0));
}

void Interpreter::lengthList()
{
    std::auto_ptr<ListVar> listVar = popStack<ListVar>();
    IntVar::value_type size = listVar->contents()->size();
    
    pushStack(listVar);
    pushStack(std::auto_ptr<Variant>(m_execState->intFactory().createValue(size)));
}


void Interpreter::appendList()
{
    boost::shared_ptr<Variant> valueVar(popStack<Variant>().release());
    std::auto_ptr<ListVar> listVar(popStack<ListVar>());

    listVar->appendValue(valueVar);
    pushStack(listVar);
}

void Interpreter::indexList()
{
    std::auto_ptr<IntVar> indexVar(popStack<IntVar>());
    std::auto_ptr<ListVar> listVar(popStack<ListVar>());

    
    MOOVE_ASSERT(indexVar->value() >= 1 && indexVar->value() <= listVar->contents()->size(), "index out of range");

    pushStack(std::auto_ptr<Variant>((*listVar->contents())[indexVar->value() - 1]->clone()));
}

void Interpreter::setIndexList()
{
    std::auto_ptr<Variant> valueVar = popStack<Variant>();
    std::auto_ptr<IntVar> indexVar = popStack<IntVar>();
    std::auto_ptr<ListVar> listVar = popStack<ListVar>();
    
    MOOVE_ASSERT(indexVar->value() >= 1 && indexVar->value() <= listVar->contents()->size(), "index out of range");

    listVar->setIndex(indexVar->value() - 1, boost::shared_ptr<Variant>(valueVar));
    pushStack(listVar);
}    

void Interpreter::rangeList()
{
    std::auto_ptr<IntVar> endVar(popStack<IntVar>());
    std::auto_ptr<IntVar> startVar(popStack<IntVar>());
    std::auto_ptr<ListVar> listVar(popStack<ListVar>());

    MOOVE_ASSERT(startVar->value() >= 1 && startVar->value() <= listVar->contents()->size(), "index out of range");
    MOOVE_ASSERT(endVar->value() >= startVar->value() && endVar->value() <= listVar->contents()->size(), "index out of range");

    ListVar::Container range;

    for(ListVar::Container::size_type i = startVar->value() - 1; i < endVar->value(); ++i)
        range.push_back(boost::shared_ptr<Variant>((*listVar->contents())[i]->clone()));

    pushStack(std::auto_ptr<Variant>(m_execState->listFactory().createList(range)));
}

void Interpreter::spliceList()
{
    std::auto_ptr<ListVar> srcListVar(popStack<ListVar>());
    std::auto_ptr<ListVar> destListVar(popStack<ListVar>());

    ListVar::Container contents(*destListVar->contents());
    contents.insert(contents.end(), srcListVar->contents()->begin(), srcListVar->contents()->end());

    destListVar->setContents(contents);
    pushStack(destListVar);
}

void Interpreter::stepInstruction()
{
    assert(m_execState);

    try {
        MOOVE_ASSERT(m_curVect != 0, "NULL current CodeVector");

        if(!execFinished()) {
            Reply reply;

            Opcode op = static_cast<Opcode>(*m_execPos++);
            MOOVE_ASSERT(OpcodeInfo::validOp(op), "invalid opcode");

            const OpcodeInfo::Op& opInfo = OpcodeInfo::getOp(op);
            if(opInfo.isUnaryOp()) {
                std::auto_ptr<Variant> operand(popStack<Variant>());

                boost::shared_ptr<OperatorPackage> opPkg = m_execState->operatorMap().findUnary(operand->factory().regEntry());
                MOOVE_ASSERT(opPkg, "unhandled unary operation");

                reply = opPkg->doUnaryOp(op, operand);
                MOOVE_ASSERT(reply.normal(), "error occured in unary operation");

                pushStack(std::auto_ptr<Variant>(reply.value()->clone()));
            } else if(opInfo.isBinaryOp()) {
                std::auto_ptr<Variant> rightOperand(popStack<Variant>());
                std::auto_ptr<Variant> leftOperand(popStack<Variant>());

                boost::shared_ptr<OperatorPackage> opPkg;
                opPkg = m_execState->operatorMap().findBinary(leftOperand->factory().regEntry(), 
                                                              rightOperand->factory().regEntry());
                MOOVE_ASSERT(opPkg != 0, "unhandled binary operation");

                reply = opPkg->doBinaryOp(op, leftOperand, rightOperand);
                MOOVE_ASSERT(reply.normal(), "error occured in binary operation");
	    
                pushStack(std::auto_ptr<Variant>(reply.value()->clone()));
            } else {
                CodeVector::Word imm;

                if (opInfo.hasImmediate()) {
                    if (opInfo.immediateType() == ImmediateValue::LABEL) {
                        imm = m_curVect->unpackWord(m_execPos, m_curVect->labelSize());
                        m_execPos += m_curVect->labelSize();
                    } else {
                        imm = m_curVect->unpackWord(m_execPos, m_bc->immediateSize(opInfo.immediateType()));
                        m_execPos += m_bc->immediateSize(opInfo.immediateType());
                    }
                }		       

                switch(op) {
                    case OP_DONE:
                        m_retVal.reset(m_execState->intFactory().createValue(0));
                        break;

                    case OP_JUMP_FALSE:
                    {
                        std::auto_ptr<Variant> testVar = popStack<Variant>();
                        if(testVar->truthValue()) {
                            // value is true, continue executing next instruction
                            break;
                        }

                        // value is false, fall throuth to jump case
                    }

                    case OP_JUMP:
                        m_execPos = m_curVect->begin() + imm;
                        break;

                    case OP_RETURN:
                        m_retVal = popStack<Variant>();
                        break;
		    
                    case OP_PUSH_LITERAL:
                        // don't push a value if it will be immediately popped
                        if (execFinished() || *m_execPos != OP_POP) {
                            pushStack(std::auto_ptr<Variant>(m_bc->literal(imm).clone()));
                        } else {
                            // skip OP_POP
                            ++m_execPos;
                        }

                        break;
	    
                    case OP_PUSH:
                        MOOVE_ASSERT(imm < m_temps.size(), "temporary ID out of range");
                        MOOVE_ASSERT(!m_temps.is_null(imm), "undefined variable");

                        // don't push a value if it will be immediately popped
                        if (execFinished() || *m_execPos != OP_POP) {
                            pushStack(std::auto_ptr<Variant>(m_temps[imm].clone()));
                        } else {
                            // skip OP_POP
                            ++m_execPos;
                        }

                        break;

                    case OP_PUT:
                        MOOVE_ASSERT(imm < m_temps.size(), "temporary ID out of range");
                        m_temps.replace(m_temps.begin() + imm, m_stack.back().clone());
                        break;

                    case OP_POP:
                        popStack<Variant>();
                        break;
                    
                    case OP_INDEX:
                        indexList();
                        break;

                    case OP_INDEX_SET:
                        setIndexList();
                        break;

                    case OP_RANGE:
                        rangeList();
                        break;

                    case OP_SPLICE:
                        spliceList();
                        break;

                    case OP_LENGTH:
                        lengthList();
                        break;

                    case OP_APPEND_LIST:
                        appendList();
                        break;

                    case OP_CALL_BUILTIN:
                        execBuiltin();
                        break;

                    default:
                        MOOVE_THROW((std::string)"not yet implemented: " + opInfo.name());
                }
            }

            if (m_traceFlag) {
                std::cerr << opInfo.name() << std::endl;
                dumpStack(std::cerr);
            }
        }
    } catch (...) {
        std::cerr << "Error occured at BC position " << (m_execPos - m_curVect->begin()) << std::endl;

        std::cerr << "\nStack dump:\n";
        dumpStack(std::cerr);
        throw;
    }
}

Interpreter::Interpreter(const DebugBytecodeProgram& bc) : m_execState(0), m_curVect(0), m_traceFlag(false)
{
    m_bc.reset(m_bcDebug = new DebugBytecodeProgram(bc));

    // Setup with correct number of NULL initial values
    for(TemporaryValues::size_type i = 0; i < 1000; ++i)
        m_temps.push_back(0);
}

std::auto_ptr<Variant> Interpreter::run(ExecutionState& execState,
                                        VariableDefMap& varDefs,
                                        bool traceFlag)
{
    m_retVal.reset();
    m_stack.clear();

    m_execState = &execState;
    m_curVect = &m_bc->forkVector(0);
    m_execPos = m_curVect->begin();
    m_traceFlag = traceFlag;

    defineVariables(varDefs);

    while(!execFinished())
	stepInstruction();

    MOOVE_ASSERT(m_retVal.get() != 0, "no return value");
    return m_retVal;
}

}	// namespace Moove
