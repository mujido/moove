#include "compiler.hpp"
#include "immediate.hpp"
#include "label_linker.hpp"
#include "program_ast.hpp"

#include <iostream>
#include <iomanip>

namespace Moove {

void Compiler::setAddr(LabelLinker& label)
{
    label.setAddr(currentAddr(), m_immCounts);
}

void Compiler::emitOp(Opcode op)
{
    m_codeVect.push_back(op);
}

ImmediateValue& Compiler::emitImmWithoutCounting(ImmediateValue::Type type, Word value)
{
    boost::shared_ptr<ImmediateValue> imm(new ImmediateValue(currentAddr(), type, value));
    m_imms.push_back(imm);

    return *m_imms.back();
}

ImmediateValue& Compiler::emitImm(ImmediateValue::Type type, Word value)
{
    ImmediateValue& imm = emitImmWithoutCounting(type, value);
    ++m_immCounts[type];
    return imm;
}

void Compiler::emitLabel(LabelLinker& label)
{
    label.addLabel(emitImmWithoutCounting(ImmediateValue::LABEL), m_immCounts);
    ++m_immCounts[ImmediateValue::LABEL];
}

Compiler::Temp Compiler::findTemp(Symbol id)
{
    VarTable::const_iterator pos = m_vars.find(id);
    if(pos != m_vars.end())
        return pos->second;
    else {
        Temp tmp = createTemp();
        m_vars[id] = tmp;
        return tmp;
    } 
}

void Compiler::pushVar(Symbol id)
{
    pushTemp(findTemp(id));
}

void Compiler::putVar(Symbol id)
{
    putTemp(findTemp(id));
}

Compiler::Temp Compiler::createTemp()
{
    return m_nextTempValue++;
}

void Compiler::pushTemp(Temp tmp)
{
    emitOp(OP_PUSH);
    emitImm(ImmediateValue::TEMP, tmp);
}

void Compiler::putTemp(Temp tmp)
{
    emitOp(OP_PUT);
    emitImm(ImmediateValue::TEMP, tmp);
}

void Compiler::incTemp(Temp tmp)
{
    std::auto_ptr<Variant> literal(m_intFactory->createValue(1));
   
    pushTemp(tmp);
    pushLiteral(literal);
    emitOp(OP_ADD);
    putTemp(tmp);
}

void Compiler::pushLiteral(std::auto_ptr<Variant> literal)
{
    m_literals.push_back(literal.release());
    
    emitOp(OP_PUSH_LITERAL);
    emitImm(ImmediateValue::LITERAL, m_literals.size() - 1);
}

void Compiler::pushString(const std::string& str)
{
    std::auto_ptr<Variant> var(m_strFactory->createValue(str));
    pushLiteral(var);
}

void Compiler::pushList(const Expr::ArgList& arglist)
{
    pushLiteral(std::auto_ptr<Variant>(m_listFactory->create()));

    m_didSplice = false;

    Expr::ArgList::const_iterator end = arglist.end();
    for(Expr::ArgList::const_iterator elem = arglist.begin(); elem != end; ++elem) {
        (*elem)->accept(*this);
        
        if (!m_didSplice)
            emitOp(OP_APPEND_LIST);
        else 
            m_didSplice = false;
    }
}

void Compiler::unaryExpr(Opcode op, const Expr::UnaryExpr& expr)
{
    expr.operand().accept(*this);
    emitOp(op);
}
   
void Compiler::binaryExpr(Opcode op, const Expr::BinaryExpr& expr)
{
    expr.left().accept(*this);
    expr.right().accept(*this);
    emitOp(op);
}

void Compiler::preAssign(const Expr::Expr& dest)
{
    // Must handle more complex assignment expressions specially. The left side must be evaluated first to preserve 
    // proper evaluation order.
    if (const Expr::Prop* propExpr = dynamic_cast<const Expr::Prop*>(&dest)) {
        propExpr->object().accept(*this);
        propExpr->name().accept(*this);
    } else if (const Expr::Index* indexExpr = dynamic_cast<const Expr::Index*>(&dest)) {
        preAssign(indexExpr->expr());
        indexExpr->expr().accept(*this);
        indexExpr->index().accept(*this);
    } else if (const Expr::Range* rangeExpr = dynamic_cast<const Expr::Range*>(&dest)) {
        preAssign(indexExpr->expr());
        rangeExpr->expr().accept(*this);
        rangeExpr->start().accept(*this);
        rangeExpr->end().accept(*this);
    }
}
    
void Compiler::assignTo(const Expr::Expr& dest)
{
    if (const Expr::Variable* varExpr = dynamic_cast<const Expr::Variable*>(&dest)) {
        putVar(varExpr->id());
    } else if (const Expr::Prop* propExpr = dynamic_cast<const Expr::Prop*>(&dest)) {
        emitOp(OP_PUT_PROP);
    } else if (const Expr::Index* indexExpr = dynamic_cast<const Expr::Index*>(&dest)) {
        // Must store calculated value into a temporary so that if there are chained assignments such as x = y[3] = z,
        // x = z not y after y[3] = z is executed.
        Temp value = createTemp();

        putTemp(value);
        emitOp(OP_INDEX_SET);
        assignTo(indexExpr->expr());
        emitOp(OP_POP);
        pushTemp(value);
    } else if (const Expr::Range* rangeExpr = dynamic_cast<const Expr::Range*>(&dest)) {
        // Must store calculated value into a temporary so that if there are chained assignments such as x = y[3] = z,
        // x = z not y after y[3] = z is executed.
        Temp value = createTemp();

        putTemp(value);
        emitOp(OP_RANGE_SET);
        assignTo(rangeExpr->expr());
        emitOp(OP_POP);
        pushTemp(value);
    } else {
        MOOVE_THROW("unexpected lvalue in assignment expression");
    }
}

void Compiler::opAssignTo(Opcode op, const Expr::Expr& left, const Expr::Expr& right)
{
    preAssign(left);
    left.accept(*this);
    right.accept(*this);
    emitOp(op);
    assignTo(left);
}

void Compiler::jump(LabelLinker& label)
{
    emitOp(OP_JUMP);
    emitLabel(label);
}

void Compiler::jumpFalse(LabelLinker& label)
{
    emitOp(OP_JUMP_FALSE);
    emitLabel(label);
}

void Compiler::ifElse(const Expr::Expr& test, const Stmt::Block& body, LabelLinker& falseLbl)
{
    test.accept(*this);
    jumpFalse(falseLbl);
    acceptAll(body);
}

void Compiler::beginLoop(Symbol id)
{
    m_flow.beginBlock(FlowControl::LOOP, id, currentAddr(), m_immCounts);
}

void Compiler::endLoop()
{
    m_flow.endBlock(currentAddr(), m_immCounts);
}

void Compiler::setContinueAddr()
{
    m_flow.setContinueAddr(currentAddr(), m_immCounts);
}

void Compiler::jumpLoopBegin(Symbol id)
{
    emitOp(OP_JUMP);
    m_flow.addBeginLabel(emitImmWithoutCounting(ImmediateValue::LABEL), id, m_immCounts);
    ++m_immCounts[ImmediateValue::LABEL];
}

void Compiler::jumpLoopContinue(Symbol id)
{
    emitOp(OP_JUMP);
    m_flow.addContinueLabel(emitImmWithoutCounting(ImmediateValue::LABEL), id, m_immCounts);
    ++m_immCounts[ImmediateValue::LABEL];
}

void Compiler::jumpLoopEnd(Symbol id)
{
    emitOp(OP_JUMP);
    m_flow.addEndLabel(emitImmWithoutCounting(ImmediateValue::LABEL), id, m_immCounts);
    ++m_immCounts[ImmediateValue::LABEL];
}   

void Compiler::jumpFalseLoopEnd(Symbol id)
{
    emitOp(OP_JUMP_FALSE);
    m_flow.addEndLabel(emitImmWithoutCounting(ImmediateValue::LABEL), id, m_immCounts);
    ++m_immCounts[ImmediateValue::LABEL];
}

Compiler::Word Compiler::calcImmSizes(Word cvSize, ImmediateValue::Deltas& immBytes)
{
    // calculate sizes of various immediate values. for labels, assume 1 byte for now and later recalculate
    immBytes[ImmediateValue::LABEL] =   0;
    immBytes[ImmediateValue::TEMP] =    CodeVector::packedWordSize(m_nextTempValue - 1);
    immBytes[ImmediateValue::LITERAL] = CodeVector::packedWordSize(m_literals.size() - 1);
    immBytes[ImmediateValue::FORK] =    CodeVector::packedWordSize(1);

    // calculate effective code size including immediate values
    Word effectiveSize = cvSize + sumImmByteDeltas(m_immCounts, immBytes);
   
    // loop until a sufficiently-sized label is found
    Word newLabelSize = 1;
    do {
        immBytes[ImmediateValue::LABEL] = newLabelSize;
        
        // calculate new code vector size given a label size of newLabelSize bytes
        newLabelSize = CodeVector::packedWordSize(effectiveSize + newLabelSize * m_immCounts[ImmediateValue::LABEL] - 1);
    } while(newLabelSize > immBytes[ImmediateValue::LABEL]);

    return effectiveSize + immBytes[ImmediateValue::LABEL] * m_immCounts[ImmediateValue::LABEL];
}

CodeVector::difference_type 
Compiler::sumImmByteDeltas(const ImmediateValue::Deltas& immCounts, const ImmediateValue::Deltas& immBytes)
{
    return (immCounts[ImmediateValue::LABEL]    * immBytes[ImmediateValue::LABEL]       +
            immCounts[ImmediateValue::TEMP]     * immBytes[ImmediateValue::TEMP]        +
            immCounts[ImmediateValue::LITERAL]  * immBytes[ImmediateValue::LITERAL]     +
            immCounts[ImmediateValue::FORK]     * immBytes[ImmediateValue::FORK]);
}   

std::auto_ptr<BytecodeProgram::ForkVector> Compiler::link(const TempCodeVector& cv, ImmediateValue::Deltas& immBytes)
{
    Word newCVSize = calcImmSizes(cv.size(), immBytes);
   
    std::auto_ptr<BytecodeProgram::ForkVector> forkVect(new BytecodeProgram::ForkVector(1));
    (*forkVect)[0].resize(newCVSize);

    // set all previous immediate value counters to 0
    ImmediateValue::Deltas prevCounts;
    prevCounts.assign(0);

    Word oldCVSize = cv.size();
    CodeVector::iterator out = (*forkVect)[0].begin();
    for(TempCodeVector::size_type cvIndex = 0; cvIndex <= oldCVSize; ++cvIndex, ++out) {
        // look into immediate value queue and emit in order any values whose
        // position points to the current location
        while(!m_imms.empty() && m_imms.front()->pos() == cvIndex) {
            ImmediateValue& imm = *m_imms.front();
            Word value = imm.value();

            if (imm.type() == ImmediateValue::LABEL) {
                // add byte delta incurred by previous immediate values
                value += sumImmByteDeltas(prevCounts, immBytes);
            
                // add byte delta incurred between label position and destination. This can be negative if the destination is before
                // the label.
                value += sumImmByteDeltas(imm.immDeltas(), immBytes);

                // assert the label does not point past the end of the code vector
                assert(value < newCVSize);
            }

            // assert that adding the immediate value does not overrun the code vector
            assert(out - (*forkVect)[0].begin() + immBytes[imm.type()] < newCVSize);

            (*forkVect)[0].packWord(out, value, immBytes[imm.type()]);
            out += immBytes[imm.type()];
         
            ++prevCounts[imm.type()];
            m_imms.pop_front();
        }

        if (cvIndex < oldCVSize) {
            // assert there is room for this opcode
            assert(out - (*forkVect)[0].begin() < newCVSize);

            *out = cv[cvIndex];
        }
    }

    return forkVect;
}

void Compiler::reset()
{
    m_nextTempValue = 0;
    m_imms.clear();
    m_literals.clear();
    m_vars.clear();
    m_codeVect.clear();

    std::fill(m_immCounts.begin(), m_immCounts.end(), 0);
}

Compiler::Compiler(const TypeRegistry& typeReg) : m_typeReg(&typeReg)
{
    // lookup required types
    const TypeRegistry::TypeEntry* intType = m_typeReg->findType("int");
    const TypeRegistry::TypeEntry* realType = m_typeReg->findType("real");
    const TypeRegistry::TypeEntry* strType = m_typeReg->findType("str");
    const TypeRegistry::TypeEntry* listType = m_typeReg->findType("list");

    const char *missingType = 0;

    if(!intType)
        missingType = "int";
    else if(!realType)
        missingType = "real";
    else if(!strType)
        missingType = "str";
    else if (!listType)
        missingType = "list";

    if(missingType)
        throw CompileError("Compiler", "Compiler", std::string("Missing required type in Moove::TypeRegistry: ") + missingType);

    // cache factories for required types
    m_intFactory = dynamic_cast<IntVar::Factory*>(&intType->factory());
    m_realFactory = dynamic_cast<RealVar::Factory*>(&realType->factory());
    m_strFactory = dynamic_cast<StrVar::Factory*>(&strType->factory());
    m_listFactory = dynamic_cast<ListVar::Factory*>(&listType->factory());

    if(!m_intFactory)
        missingType = "int";
    else if(!m_realFactory)
        missingType = "real";
    else if(!m_strFactory)
        missingType = "str";
    else if(!m_listFactory)
        missingType = "list";
    else
        missingType = 0;

    if(missingType)
        throw CompileError("Compiler", "Compiler", std::string("Required type not descended from correct parent: ") + missingType);
}

std::auto_ptr<BytecodeProgram> Compiler::compile(const Program& prog)
{
    reset();

    // accept all statements and terminate code vector with an OP_DONE
    acceptAll(prog.stmts());
    emitOp(OP_DONE);

    ImmediateValue::Deltas immBytes;

    return std::auto_ptr<BytecodeProgram>(new BytecodeProgram(link(m_codeVect, immBytes),
                                                              m_literals,
                                                              immBytes[ImmediateValue::TEMP],
                                                              immBytes[ImmediateValue::LITERAL],
                                                              immBytes[ImmediateValue::FORK]));
}

std::auto_ptr<DebugBytecodeProgram> Compiler::compileDebug(const Program& prog)
{
    reset();
   
    // accept all statements and terminate code vector with an OP_DONE
    acceptAll(prog.stmts());
    emitOp(OP_DONE);

    DebugBytecodeProgram::SymbolIDMap varIDMap(m_nextTempValue);

    // create mapping between temporary IDs and variables
    VarTable::const_iterator varsEnd = m_vars.end();
    for (VarTable::const_iterator varAssoc = m_vars.begin(); varAssoc != varsEnd; ++varAssoc)
        varIDMap[varAssoc->second] = varAssoc->first;
    
    ImmediateValue::Deltas immBytes;

    return std::auto_ptr<DebugBytecodeProgram>(new DebugBytecodeProgram(link(m_codeVect, immBytes),
                                                                        m_literals,
                                                                        prog.varTable(),
                                                                        varIDMap,
                                                                        immBytes[ImmediateValue::TEMP],
                                                                        immBytes[ImmediateValue::LITERAL],
                                                                        immBytes[ImmediateValue::FORK]));
}
      
void Compiler::visit(const Program&)
{
    // not yet implemented
}
   
void Compiler::visit(const Expr::Integer& intExpr)
{
    std::auto_ptr<Variant> var(m_intFactory->createValue(intExpr.value()));
    pushLiteral(var);
}

void Compiler::visit(const Expr::Objnum& objnumExpr)
{
    // not yet implemented
}

void Compiler::visit(const Expr::Str& strExpr)
{
    std::auto_ptr<Variant> var(m_strFactory->createValue(strExpr.str()));
    pushLiteral(var);
}

void Compiler::visit(const Expr::Real& realExpr)
{
    std::auto_ptr<Variant> var(m_realFactory->createValue(realExpr.value()));
    pushLiteral(var);
}

void Compiler::visit(const Expr::List& listExpr)
{
    pushList(listExpr.elements());
}
 
void Compiler::visit(const Expr::Variable& varExpr)
{
    pushVar(varExpr.id());
}

void Compiler::visit(const Expr::Scatter& scatterExpr)
{
    // not yet implemented
}

void Compiler::visit(const Expr::Scatter::Target& target)
{
    //Unimplemented; handled in visit(const Expr::Scatter&)
}

void Compiler::visit(const Expr::Not& expr)
{ 
    unaryExpr(OP_NOT, expr); 
}

void Compiler::visit(const Expr::Negate& expr)
{ 
    unaryExpr(OP_NEGATE, expr); 
}

void Compiler::visit(const Expr::Splice& expr)
{ 
    unaryExpr(OP_SPLICE, expr); 
    m_didSplice = true;
}

void Compiler::visit(const Expr::PreInc& expr)
{
    std::auto_ptr<Variant> literal(m_intFactory->createValue(1));

    preAssign(expr.operand());
    expr.operand().accept(*this);
    pushLiteral(literal);
    emitOp(OP_ADD);
    assignTo(expr.operand());
}

void Compiler::visit(const Expr::PreDec& expr)
{
    std::auto_ptr<Variant> literal(m_intFactory->createValue(1));

    preAssign(expr.operand());
    expr.operand().accept(*this);
    pushLiteral(literal);
    emitOp(OP_SUB);
    assignTo(expr.operand());
}

void Compiler::visit(const Expr::PostInc& expr)
{
    Temp origTemp = createTemp();
    std::auto_ptr<Variant> literal(m_intFactory->createValue(1));

    preAssign(expr.operand());
    expr.operand().accept(*this);
    putTemp(origTemp);
    pushLiteral(literal);
    emitOp(OP_ADD);
    assignTo(expr.operand());
    emitOp(OP_POP);
    pushTemp(origTemp);
}

void Compiler::visit(const Expr::PostDec& expr)
{
    Temp origTemp = createTemp();
    std::auto_ptr<Variant> literal(m_intFactory->createValue(1));

    preAssign(expr.operand());
    expr.operand().accept(*this);
    putTemp(origTemp);
    pushLiteral(literal);
    emitOp(OP_SUB);
    assignTo(expr.operand());
    emitOp(OP_POP);
    pushTemp(origTemp);
}

void Compiler::visit(const Expr::Or& expr)
{ 
    binaryExpr(OP_OR, expr); 
}

void Compiler::visit(const Expr::And& expr)
{ 
    binaryExpr(OP_AND, expr); 
}

void Compiler::visit(const Expr::Equal& expr) 
{ 
    binaryExpr(OP_EQ, expr); 
}
                   
void Compiler::visit(const Expr::NotEqual& expr) 
{ 
    binaryExpr(OP_NE, expr); 
}

void Compiler::visit(const Expr::Less& expr) 
{ 
    binaryExpr(OP_LT, expr); 
}

void Compiler::visit(const Expr::LessEqual& expr) 
{ 
    binaryExpr(OP_LE, expr); 
}

void Compiler::visit(const Expr::GreaterEqual& expr) 
{ 
    binaryExpr(OP_GE, expr); 
}

void Compiler::visit(const Expr::Greater& expr) 
{ 
    binaryExpr(OP_GT, expr); 
}

void Compiler::visit(const Expr::In& expr) 
{ binaryExpr(OP_IN, expr); } 

void Compiler::visit(const Expr::Add& expr) 
{ 
    binaryExpr(OP_ADD, expr); 
}

void Compiler::visit(const Expr::Sub& expr) 
{ 
    binaryExpr(OP_SUB, expr); 
}

void Compiler::visit(const Expr::Mul& expr) 
{ 
    binaryExpr(OP_MUL, expr); 
}

void Compiler::visit(const Expr::Div& expr)
{ 
    binaryExpr(OP_DIV, expr); 
}

void Compiler::visit(const Expr::Mod& expr) 
{ 
    binaryExpr(OP_MOD, expr); 
}
   
void Compiler::visit(const Expr::Exp& expr)
{ 
    binaryExpr(OP_EXP, expr); 
}

void Compiler::visit(const Expr::Assign& expr)
{
    preAssign(expr.left());
    expr.right().accept(*this);
    assignTo(expr.left());
}
   
void Compiler::visit(const Expr::AddEqual& expr) 
{ 
    opAssignTo(OP_ADD, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::SubEqual& expr) 
{ 
    opAssignTo(OP_SUB, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::MulEqual& expr) 
{ 
    opAssignTo(OP_MUL, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::DivEqual& expr) 
{ 
    opAssignTo(OP_DIV, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::ModEqual& expr) 
{ 
    opAssignTo(OP_MOD, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::ExpEqual& expr) 
{ 
    opAssignTo(OP_EXP, expr.left(), expr.right()); 
}

void Compiler::visit(const Expr::Conditional& condExpr)
{
    LabelLinker falseLbl;
    LabelLinker endLbl;

    condExpr.test().accept(*this);
    jumpFalse(falseLbl);
    condExpr.trueExpr().accept(*this);
    jump(endLbl);
    setAddr(falseLbl);
    condExpr.falseExpr().accept(*this);
    setAddr(endLbl);
}

void Compiler::visit(const Expr::Index& indexExpr)
{
    indexExpr.expr().accept(*this);
    indexExpr.index().accept(*this);
    emitOp(OP_INDEX);
}

void Compiler::visit(const Expr::Range& rangeExpr)
{
    rangeExpr.expr().accept(*this);
    rangeExpr.start().accept(*this);
    rangeExpr.end().accept(*this);
    emitOp(OP_RANGE);
}

void Compiler::visit(const Expr::Catch& catchExpr)
{
    // not yet implemented
}

void Compiler::visit(const Expr::Prop& propExpr)
{
    propExpr.object().accept(*this);
    propExpr.name().accept(*this);
    emitOp(OP_GET_PROP);
}

void Compiler::visit(const Expr::SystemProp& sysPropExpr)
{
    pushString(sysPropExpr.name());
    emitOp(OP_GET_SYSPROP);
}

void Compiler::visit(const Expr::SystemCall& sysCallExpr)
{
    pushString(sysCallExpr.name());
    pushList(sysCallExpr.args());
    emitOp(OP_CALL_SYSVERB);
}

void Compiler::visit(const Expr::VerbCall& verbCallExpr)
{
    verbCallExpr.object().accept(*this);
    verbCallExpr.name().accept(*this);
    pushList(verbCallExpr.args());
    emitOp(OP_CALL_VERB);
}

void Compiler::visit(const Expr::Builtin& builtinExpr)
{
    pushString(builtinExpr.name());
    pushList(builtinExpr.args());
    emitOp(OP_CALL_BUILTIN);
}

void Compiler::visit(const Expr::Length&)
{
    emitOp(OP_LENGTH);
}

void Compiler::visit(const Stmt::If& ifStmt)
{
    LabelLinker ifEndLbl;
    LabelLinker ifFalseLbl;

    ifElse(ifStmt.test(), ifStmt.body(), ifFalseLbl);
   
    // jump to end of if statement if there are else clauses
    if(!ifStmt.elseList().empty()) {
        jump(ifEndLbl);
    }

    setAddr(ifFalseLbl);

    Stmt::If::ElseList::const_iterator elseEnd = ifStmt.elseList().end();
    for(Stmt::If::ElseList::const_iterator elseClause = ifStmt.elseList().begin(); elseClause != elseEnd; ++elseClause) {
        LabelLinker elseFalseLbl;

        if((*elseClause)->hasTest()) {
            // elseif
            ifElse((*elseClause)->test(), (*elseClause)->body(), elseFalseLbl);
        } else {
            // else
            acceptAll((*elseClause)->body());
        }

        if(elseClause + 1 < elseEnd) {
            // this is not the last else(if) clause, so jump to end of if statement
            jump(ifEndLbl);
        }
      
        setAddr(elseFalseLbl);
    }

    setAddr(ifEndLbl);
}

void Compiler::visit(const Stmt::If::ElseIf& elseClause)
{
    //Unused; Stmt::If Compiler::visitor takes care of else clauses
}
         
void Compiler::visit(const Stmt::While& whileStmt)
{
    beginLoop(whileStmt.id());
    setContinueAddr();
    whileStmt.test().accept(*this);
    jumpFalseLoopEnd();
      
    acceptAll(whileStmt.body());

    jumpLoopBegin();
    endLoop();
}

void Compiler::visit(const Stmt::ForList& forStmt)
{
    Temp listTmp = createTemp();
    Temp indexTmp = createTemp();
    Temp lengthTmp = createTemp();

    // Start indexing list at 1. Store into indexTmp
    std::auto_ptr<Variant> indexLiteral(m_intFactory->createValue(1));
    pushLiteral(indexLiteral);
    putTemp(indexTmp);

    // push list onto stack and store into listTmp
    forStmt.expr().accept(*this);
    putTemp(listTmp);
   
    // list still remains on stack. Take length and store it into lengthTmp. Pop both values off stack.
    emitOp(OP_LENGTH);
    putTemp(lengthTmp);
    emitOp(OP_POP);
    emitOp(OP_POP);

    // Start index remains on stack from beginning. Push length value back onto stack for comparison
    pushTemp(lengthTmp);
 
    // mark beginning of loop
    beginLoop(forStmt.id());

    emitOp(OP_LE);
    jumpFalseLoopEnd();

    // load list[index] into for statement's loop variable
    pushTemp(listTmp);
    pushTemp(indexTmp);
    emitOp(OP_INDEX);
    putVar(forStmt.id());
    emitOp(OP_POP);

    acceptAll(forStmt.body());

    // continue loop at this point. Increment index variable and jump back to loop beginning.
    setContinueAddr();
    incTemp(indexTmp);
    pushTemp(lengthTmp);
    jumpLoopBegin();

    endLoop();
}

void Compiler::visit(const Stmt::ForRange& forStmt)
{
    Temp indexTmp = createTemp();
    Temp endTmp = createTemp();

    forStmt.start().accept(*this);
    putTemp(indexTmp);
    forStmt.end().accept(*this);
    putTemp(endTmp);

    beginLoop(forStmt.id());

    emitOp(OP_LE);
    jumpFalseLoopEnd();

    pushTemp(indexTmp);
    putVar(forStmt.id());
    emitOp(OP_POP);

    acceptAll(forStmt.body());

    setContinueAddr();

    // increment the index temporary. leave it on the stack and push the end index temporary for the next loop iteration
    incTemp(indexTmp);
    pushTemp(endTmp);
    jumpLoopBegin();

    endLoop();
}

void Compiler::visit(const Stmt::TryExcept& tryStmt)
{
    // not yet implemented
}

void Compiler::visit(const Stmt::TryExcept::Except& except)
{
    //Unimplemented; function performed in Stmt::TryExcept Compiler::visitor
}

void Compiler::visit(const Stmt::TryFinally& tryStmt)
{
    // not yet implemented
}

void Compiler::visit(const Stmt::Fork& forkStmt)
{
    // not yet implemented
}

void Compiler::visit(const Stmt::Switch& switchStmt)
{
    // not yet implemented
}

void Compiler::visit(const Stmt::Switch::Case& caseClause)
{
    // not yet implemented
}

void Compiler::visit(const Stmt::Return& returnStmt)
{
    if (returnStmt.hasExpr()) {
        returnStmt.expr().accept(*this);
    } else {
        std::auto_ptr<Variant> retval(m_intFactory->createValue(0));
        pushLiteral(retval);
    }

    emitOp(OP_RETURN);
}

void Compiler::visit(const Stmt::Continue& continueStmt)
{
    jumpLoopContinue(continueStmt.id());
}

void Compiler::visit(const Stmt::Break& breakStmt)
{
    jumpLoopEnd(breakStmt.id());
}

void Compiler::visit(const Stmt::Expr& stmt)
{
    stmt.expr().accept(*this);
    emitOp(OP_POP);
}

}       //namespace Moove
