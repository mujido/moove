#pragma once

#include "ast_visitor.hpp"

namespace Moove {
    namespace BisonParser {
        class parser;
    }

    class ScatterAssignmentConverter : public ASTVisitor {
    public:
        using parser = BisonParser::parser;

        virtual void visit(const Program& program) override;

        virtual void visit(const Expr::Integer&) override;

        virtual void visit(const Expr::Objnum&) override;

        virtual void visit(const Expr::Str&) override;

        virtual void visit(const Expr::Real&) override;

        virtual void visit(const Expr::List&) override;

        virtual void visit(const Expr::Variable&) override;

        virtual void visit(const Expr::Scatter&) override;

        virtual void visit(const Expr::Scatter::Target&) override;

        virtual void visit(const Expr::Not&) override;

        virtual void visit(const Expr::Negate&) override;

        virtual void visit(const Expr::Splice& spliceExpr) override;

        virtual void visit(const Expr::PreInc&) override;

        virtual void visit(const Expr::PreDec&) override;

        virtual void visit(const Expr::PostInc&) override;

        virtual void visit(const Expr::PostDec&) override;

        virtual void visit(const Expr::Or&) override;

        virtual void visit(const Expr::And&) override;

        virtual void visit(const Expr::Equal&) override;

        virtual void visit(const Expr::NotEqual&) override;

        virtual void visit(const Expr::Less&) override;

        virtual void visit(const Expr::LessEqual&) override;

        virtual void visit(const Expr::GreaterEqual&) override;

        virtual void visit(const Expr::Greater&) override;

        virtual void visit(const Expr::In&) override;

        virtual void visit(const Expr::Add&) override;

        virtual void visit(const Expr::Sub&) override;

        virtual void visit(const Expr::Mul&) override;

        virtual void visit(const Expr::Div&) override;

        virtual void visit(const Expr::Mod&) override;

        virtual void visit(const Expr::Exp&) override;

        virtual void visit(const Expr::Assign&) override;

        virtual void visit(const Expr::AddEqual&) override;

        virtual void visit(const Expr::SubEqual&) override;

        virtual void visit(const Expr::MulEqual&) override;

        virtual void visit(const Expr::DivEqual&) override;

        virtual void visit(const Expr::ModEqual&) override;

        virtual void visit(const Expr::ExpEqual&) override;

        virtual void visit(const Expr::Conditional&) override;

        virtual void visit(const Expr::Index&) override;

        virtual void visit(const Expr::Range&) override;

        virtual void visit(const Expr::Catch&) override;

        virtual void visit(const Expr::Prop&) override;

        virtual void visit(const Expr::SystemProp&) override;

        virtual void visit(const Expr::SystemCall&) override;

        virtual void visit(const Expr::VerbCall&) override;

        virtual void visit(const Expr::Builtin&) override;

        virtual void visit(const Expr::Length&) override;

        virtual void visit(const Stmt::If&) override;

        virtual void visit(const Stmt::If::Else&) override;

        virtual void visit(const Stmt::While&) override;

        virtual void visit(const Stmt::ForList&) override;

        virtual void visit(const Stmt::ForRange&) override;

        virtual void visit(const Stmt::TryExcept&) override;

        virtual void visit(const Stmt::TryExcept::Except&) override;

        virtual void visit(const Stmt::TryFinally&) override;

        virtual void visit(const Stmt::Fork&) override;

        virtual void visit(const Stmt::Switch&) override;

        virtual void visit(const Stmt::Switch::Case&) override;

        virtual void visit(const Stmt::Return&) override;

        virtual void visit(const Stmt::Continue&) override;

        virtual void visit(const Stmt::Break&) override;

        virtual void visit(const Stmt::ExprStmt&) override;

        static Expr::Scatter::TargetList convert(const Expr::Expr& expr);

        static Expr::Scatter::TargetList convert(const Expr::ArgList& argList);
       
    private:
        enum State
        {
            START,
            IN_LIST,
            IN_SPLICE
        };

        Expr::Scatter::TargetList m_targets;
        State m_state = START;
    };
}
