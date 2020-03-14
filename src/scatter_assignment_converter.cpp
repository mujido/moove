#include "scatter_assignment_converter.hpp"
#include "moove.tab.h"

namespace Moove {
    void ScatterAssignmentConverter::visit(const Program& program) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Integer&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Objnum&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Str&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Real&) { throw parser::syntax_error("invalid scatter expression"); }

    void ScatterAssignmentConverter::visit(const Expr::List& listExpr)
    { 
        if (m_state != START)
            throw parser::syntax_error("invalid scatter expression"); 
        
        m_state = IN_LIST;
        for (const auto& elem : listExpr.elements())
            elem->accept(*this);
    }

    void ScatterAssignmentConverter::visit(const Expr::Variable& varExpr)
    {
        if (m_state != IN_LIST && m_state != IN_SPLICE)
            throw parser::syntax_error("invalid scatter expression");

        using Target = Expr::Scatter::Target;
        auto targetType = m_state == IN_SPLICE ? Target::REST : Target::REQUIRED;
        m_targets.emplace_back(targetType, varExpr.id());
    }

    void ScatterAssignmentConverter::visit(const Expr::Scatter&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Scatter::Target&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Not&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Negate&) { throw parser::syntax_error("invalid scatter expression"); }

    void ScatterAssignmentConverter::visit(const Expr::Splice& spliceExpr)
    {
        if (m_state != IN_LIST)
            throw parser::syntax_error("invalid scatter expression");

        try
        {
            m_state = IN_SPLICE;
            spliceExpr.operand().accept(*this);
        }
        catch (...)
        {
            m_state = IN_LIST;
            throw;
        }
    }

    void ScatterAssignmentConverter::visit(const Expr::PreInc&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::PreDec&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::PostInc&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::PostDec&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Or&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::And&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Equal&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::NotEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Less&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::LessEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::GreaterEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Greater&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::In&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Add&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Sub&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Mul&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Div&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Mod&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Exp&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Assign&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::AddEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::SubEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::MulEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::DivEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::ModEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::ExpEqual&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Conditional&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Index&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Range&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Catch&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Prop&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::SystemProp&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::SystemCall&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::VerbCall&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Builtin&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Expr::Length&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::If&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::If::Else&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::While&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::ForList&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::ForRange&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::TryExcept&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::TryExcept::Except&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::TryFinally&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Fork&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Switch&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Switch::Case&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Return&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Continue&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::Break&) { throw parser::syntax_error("invalid scatter expression"); }
    void ScatterAssignmentConverter::visit(const Stmt::ExprStmt&) { throw parser::syntax_error("invalid scatter expression"); }

    Expr::Scatter::TargetList ScatterAssignmentConverter::convert(const Expr::Expr& expr)
    {
        ScatterAssignmentConverter converter;
        expr.accept(converter);
        return std::move(converter.m_targets);
    }

    Expr::Scatter::TargetList ScatterAssignmentConverter::convert(const Expr::ArgList& argList)
    {
        ScatterAssignmentConverter converter;
        converter.m_state = IN_LIST;
        for (const auto& elem : argList)
            elem->accept(converter);

        return std::move(converter.m_targets);
    }
}