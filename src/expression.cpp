//
// Created by stanislavv on 6/15/23.
//
#include "expression.h"

template <class type, char operand>
Fraction AbstractBinary<type, operand>::eval(const std::map<std::string, Fraction> & values) const
{
    return type()(left->eval(values), right->eval(values));
}

template <class type, char operand>
std::string AbstractBinary<type, operand>::str() const
{
    return "(" + left->str() + " " + operand + " " + right->str() + ")";
}

template <class type, char operand>
Expression * AbstractBinary<type, operand>::clone() const
{
    return new AbstractBinary(*left, *right);
}

Fraction Const::eval(const std::map<std::string, Fraction> & values) const
{
    return digit;
}

Expression * Const::clone() const
{
    return new Const(*this);
}

std::string Const::str() const
{
    return digit.str();
}

Fraction Variable::eval(const std::map<std::string, Fraction> & values) const
{
    return values.at(name);
}

Expression * Variable::clone() const
{
    return new Variable(*this);
}

std::string Variable::str() const
{
    return name;
}

Fraction Negate::eval(const std::map<std::string, Fraction> & values) const
{
    return Fraction(-1) * value->eval(values);
}

Expression * Negate::clone() const
{
    return new Negate(*this);
}

std::string Negate::str() const
{
    return "(-" + value->str() + ")";
}

Add operator+(const Expression & left, const Expression & right) { return {left, right}; }

Subtract operator-(const Expression & left, const Expression & right) { return {left, right}; }

Multiply operator*(const Expression & left, const Expression & right) { return {left, right}; }

Divide operator/(const Expression & left, const Expression & right) { return {left, right}; }

Negate operator-(const Expression & digit)
{
    return {digit};
}
