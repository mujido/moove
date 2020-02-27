//Copyright (c) 2005 Kurt Stutsman. All rights reserved.
/**
 * \file
 */
#ifndef MOOVE_COMPILER_HPP
#define MOOVE_COMPILER_HPP

#include "ast_visitor.hpp"
#include "bc_program.hpp"
#include "codevect.hpp"
#include "symbol_table.hpp"
#include "opcodes.hpp"
#include "type_registry.hpp"
#include "intvar.hpp"
#include "realvar.hpp"
#include "strvar.hpp"
#include "listvar.hpp"
#include "immediate.hpp"
#include "flow_ctrl.hpp"

#include <deque>
#include <map>
#include <memory>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace Moove {

class LabelLinker;
class Program;

/// Generates bytecode from AST objects.
class Compiler : public ASTVisitor {
private:
    typedef CodeVector::Word	Word;
    
    typedef CodeVector::Byte	Byte;

    typedef Word			Temp;

    typedef std::vector<Byte>	TempCodeVector;

    typedef std::map<Symbol, Temp> VarTable;

    typedef std::deque<boost::shared_ptr<ImmediateValue> > ImmediateList;

    typedef BytecodeProgram::LiteralVector       LiteralValues;

    FlowControl		m_flow;
    ImmediateList	m_imms;
    LiteralValues	m_literals;
    VarTable		m_vars;
    Word		m_nextTempValue;
    TempCodeVector	m_codeVect;
    const TypeRegistry*	m_typeReg;

    ImmediateValue::Deltas	m_immCounts;

    // cached type factories required for compilation
    IntVar::Factory*	m_intFactory;
    RealVar::Factory*	m_realFactory;
    StrVar::Factory*	m_strFactory;
    ListVar::Factory*	m_listFactory;

    bool                m_didSplice;

    Word currentAddr()const
    { return m_codeVect.size(); }

    void setAddr(LabelLinker& label);

    void emitOp(Opcode op);

    ImmediateValue& emitImmWithoutCounting(ImmediateValue::Type type, Word value = 0);

    ImmediateValue& emitImm(ImmediateValue::Type type, Word value = 0);

    void emitLabel(LabelLinker& label);

    Temp findTemp(Symbol id);

    void pushVar(Symbol id);

    void putVar(Symbol id);

    Temp createTemp();

    void pushTemp(Temp tmp);

    void putTemp(Temp tmp);

    void incTemp(Temp tmp);

    void pushLiteral(std::unique_ptr<Variant> literal);

    void pushString(const std::string& str);

    void pushList(const Expr::ArgList& arglist);

    void unaryExpr(Opcode op, const Expr::UnaryExpr& expr);
   
    void binaryExpr(Opcode op, const Expr::BinaryExpr& expr);
   
    void preAssign(const Moove::Expr::Expr& dest);

    void assignTo(const Moove::Expr::Expr& dest);

    void opAssignTo(Opcode op, const Expr::Expr& lhs, const Expr::Expr& rhs);

    template<class Cont>
    void acceptAll(const Cont& cont)
    {
        typename Cont::const_iterator end = cont.end();
        for(typename Cont::const_iterator i = cont.begin(); i != end; ++i)
            (*i)->accept(*this);
    }

    void jump(LabelLinker& label);

    void jumpFalse(LabelLinker& label);

    void ifElse(const Expr::Expr& test, const Stmt::Block& body, LabelLinker& falseLbl);

    void beginLoop(Symbol id);

    void endLoop();

    void setContinueAddr();

    void jumpLoopBegin(Symbol id = Symbol());

    void jumpLoopContinue(Symbol id = Symbol());

    void jumpLoopEnd(Symbol id = Symbol());

    void jumpFalseLoopEnd(Symbol id = Symbol());

    Word calcImmSizes(Word cvSize, ImmediateValue::Deltas& immBytes);

    CodeVector::difference_type 
    sumImmByteDeltas(const ImmediateValue::Deltas& immCounts, const ImmediateValue::Deltas& immBytes);

    std::unique_ptr<BytecodeProgram::ForkVector> link(const TempCodeVector& cv, ImmediateValue::Deltas& immBytes);

    void reset();

public:
    typedef Exception CompileError;

    Compiler(const TypeRegistry& typeReg);

    std::unique_ptr<BytecodeProgram> compile(const Program& prog);

    std::unique_ptr<DebugBytecodeProgram> compileDebug(const Program &prog);

protected:
    void visit(const Program&);

    //Expression AST
    void visit(const Expr::Integer& intExpr);

    void visit(const Expr::Objnum& objnumExpr);

    void visit(const Expr::Str& strExpr);

    void visit(const Expr::Real& realExpr);

    void visit(const Expr::List& listExpr);

    void visit(const Expr::Variable& varExpr);

    void visit(const Expr::Scatter& scatterExpr);

    void visit(const Expr::Scatter::Target& target);

    void visit(const Expr::Not& expr);

    void visit(const Expr::Negate& expr);

    void visit(const Expr::Splice& expr);

    void visit(const Expr::PreInc& expr);

    void visit(const Expr::PreDec& expr);

    void visit(const Expr::PostInc& expr);

    void visit(const Expr::PostDec& expr);

    void visit(const Expr::Or& expr);

    void visit(const Expr::And& expr);

    void visit(const Expr::Equal& expr) ;
		   
    void visit(const Expr::NotEqual& expr) ;

    void visit(const Expr::Less& expr) ;

    void visit(const Expr::LessEqual& expr) ;

    void visit(const Expr::GreaterEqual& expr) ;

    void visit(const Expr::Greater& expr) ;

    void visit(const Expr::In& expr) ;

    void visit(const Expr::Add& expr) ;

    void visit(const Expr::Sub& expr) ;

    void visit(const Expr::Mul& expr) ;

    void visit(const Expr::Div& expr);

    void visit(const Expr::Mod& expr) ;
   
    void visit(const Expr::Exp& expr);

    void visit(const Expr::Assign& expr);

    void visit(const Expr::AddEqual& expr) ;

    void visit(const Expr::SubEqual& expr) ;

    void visit(const Expr::MulEqual& expr) ;

    void visit(const Expr::DivEqual& expr) ;

    void visit(const Expr::ModEqual& expr) ;

    void visit(const Expr::ExpEqual& expr) ;

    void visit(const Expr::Conditional& condExpr);

    void visit(const Expr::Index& indexExpr);

    void visit(const Expr::Range& rangeExpr);

    void visit(const Expr::Catch& catchExpr);

    void visit(const Expr::Prop& propExpr);

    void visit(const Expr::SystemProp& sysPropExpr);

    void visit(const Expr::SystemCall& sysCallExpr);

    void visit(const Expr::VerbCall& verbCallExpr);

    void visit(const Expr::Builtin& builtinExpr);

    void visit(const Expr::Length&);

    //Statement AST
    void visit(const Stmt::If& ifStmt);

    void visit(const Stmt::If::ElseIf& elseClause);
	 
    void visit(const Stmt::While& whileStmt);

    void visit(const Stmt::ForList& forStmt);

    void visit(const Stmt::ForRange& forStmt);

    void visit(const Stmt::TryExcept& tryStmt);

    void visit(const Stmt::TryExcept::Except& except);

    void visit(const Stmt::TryFinally& tryStmt);

    void visit(const Stmt::Fork& forkStmt);

    void visit(const Stmt::Switch& switchStmt);

    void visit(const Stmt::Switch::Case& caseClause);

    void visit(const Stmt::Return& returnStmt);

    void visit(const Stmt::Continue& continueStmt);

    void visit(const Stmt::Break& breakStmt);

    void visit(const Stmt::ExprStmt& stmt);
};

}	//namespace Moove

#endif	// MOOVE_COMPILER_HPP
