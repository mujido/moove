#include "op_package.hpp"

namespace Moove {

Reply OperatorPackage::doUnaryOp(Opcode op, std::unique_ptr<Variant> operand)const
{
    throw UndefinedOperator("OperatorPackage", "doUnaryOp", "operator undefined");
}

Reply OperatorPackage::doBinaryOp(Opcode op, std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("OperatorPackage", "doBinaryOp", "operator undefined");
}

Reply OperatorPackage::doIndexSetOp(std::unique_ptr<Variant> object, 
                                    std::unique_ptr<Variant> index, 
                                    std::unique_ptr<Variant> value)const
{
    throw UndefinedOperator("OperatorPackage", "doIndexSetOp", "operator undefined");
}

Reply OperatorPackage::doRangeOp(std::unique_ptr<Variant> object, std::unique_ptr<Variant> startIndex, std::unique_ptr<Variant> endIndex)const
{
    throw UndefinedOperator("OperatorPackage", "doRangeOp", "operator unedefined");
}

Reply OperatorPackage::doRangeSetOp(std::unique_ptr<Variant> object,
                                    std::unique_ptr<Variant> startIndex, 
                                    std::unique_ptr<Variant> endIndex,
                                    std::unique_ptr<Variant> value)const
{
    throw UndefinedOperator("OperatorPackage", "doRangeSetOp", "operator unedefined");
}

#if 0
Reply OperatorPackage::not(std::unique_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::not()"); 
}

Reply OperatorPackage::negate(std::unique_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::negate()"); 
}

Reply OperatorPackage::length(std::unique_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::length()"); 
}

Reply OperatorPackage::splice(std::unique_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::splice()"); 
}

Reply OperatorPackage::or(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::or()"); 
}

Reply OperatorPackage::and(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::and()"); 
}

Reply OperatorPackage::eq(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::eq()"); 
}

Reply OperatorPackage::ne(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::ne()"); 
}

Reply OperatorPackage::lt(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::lt()"); 
}

Reply OperatorPackage::le(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::le()"); 
}

Reply OperatorPackage::ge(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::ge()"); 
}

Reply OperatorPackage::gt(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::gt()"); 
}

Reply OperatorPackage::in(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::in()"); 
}

Reply OperatorPackage::add(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::add()"); 
}

Reply OperatorPackage::subtract(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::subtract()"); 
}

Reply OperatorPackage::multiply(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::multiply()"); 
}

Reply OperatorPackage::divide(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::divide()"); 
}

Reply OperatorPackage::modulo(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::modulo()"); 
}

Reply OperatorPackage::exponent(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::exponent()"); 
}

Reply OperatorPackage::index(std::unique_ptr<Variant> left, std::unique_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::index()"); 
}
#endif

}   //namespace Moove
