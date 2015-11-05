#include "op_package.hpp"

namespace Moove {

Reply OperatorPackage::doUnaryOp(Opcode op, std::auto_ptr<Variant> operand)const
{
    throw UndefinedOperator("OperatorPackage", "doUnaryOp", "operator undefined");
}

Reply OperatorPackage::doBinaryOp(Opcode op, std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("OperatorPackage", "doBinaryOp", "operator undefined");
}

Reply OperatorPackage::doIndexSetOp(std::auto_ptr<Variant> object, 
                                    std::auto_ptr<Variant> index, 
                                    std::auto_ptr<Variant> value)const
{
    throw UndefinedOperator("OperatorPackage", "doIndexSetOp", "operator undefined");
}

Reply OperatorPackage::doRangeOp(std::auto_ptr<Variant> object, std::auto_ptr<Variant> startIndex, std::auto_ptr<Variant> endIndex)const
{
    throw UndefinedOperator("OperatorPackage", "doRangeOp", "operator unedefined");
}

Reply OperatorPackage::doRangeSetOp(std::auto_ptr<Variant> object,
                                    std::auto_ptr<Variant> startIndex, 
                                    std::auto_ptr<Variant> endIndex,
                                    std::auto_ptr<Variant> value)const
{
    throw UndefinedOperator("OperatorPackage", "doRangeSetOp", "operator unedefined");
}

#if 0
Reply OperatorPackage::not(std::auto_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::not()"); 
}

Reply OperatorPackage::negate(std::auto_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::negate()"); 
}

Reply OperatorPackage::length(std::auto_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::length()"); 
}

Reply OperatorPackage::splice(std::auto_ptr<Variant> operand)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::splice()"); 
}

Reply OperatorPackage::or(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::or()"); 
}

Reply OperatorPackage::and(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::and()"); 
}

Reply OperatorPackage::eq(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::eq()"); 
}

Reply OperatorPackage::ne(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::ne()"); 
}

Reply OperatorPackage::lt(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::lt()"); 
}

Reply OperatorPackage::le(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::le()"); 
}

Reply OperatorPackage::ge(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::ge()"); 
}

Reply OperatorPackage::gt(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{ 
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::gt()"); 
}

Reply OperatorPackage::in(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::in()"); 
}

Reply OperatorPackage::add(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::add()"); 
}

Reply OperatorPackage::subtract(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::subtract()"); 
}

Reply OperatorPackage::multiply(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::multiply()"); 
}

Reply OperatorPackage::divide(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::divide()"); 
}

Reply OperatorPackage::modulo(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::modulo()"); 
}

Reply OperatorPackage::exponent(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::exponent()"); 
}

Reply OperatorPackage::index(std::auto_ptr<Variant> left, std::auto_ptr<Variant> right)const
{
    throw UndefinedOperator("Undefined Operator in Moove::OperatorPackage::index()"); 
}
#endif

}   //namespace Moove
