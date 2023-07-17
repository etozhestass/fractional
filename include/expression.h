//
// Created by stanislavv on 6/15/23.
//

#pragma once

#include "fractional.h"

#include <map>
#include <memory>

struct Expression
{
    virtual Fraction eval(const std::map<std::string, Fraction> & values = {}) const = 0;

    virtual Expression * clone() const = 0;

    friend std::ostream & operator<<(std::ostream & out, const Expression & expression)
    {
        return out << expression.str();
    }

    virtual ~Expression() = default;

    virtual std::string str() const = 0;
};

template <class type, char operand>
struct AbstractBinary : public Expression
{
    AbstractBinary(const Expression & left, const Expression & right)
        : left(left.clone())
        , right(right.clone())
    {
    }

    Fraction eval(const std::map<std::string, Fraction> & values = {}) const override;

    Expression * clone() const override;

    ~AbstractBinary() override = default;

    std::string str() const override;

    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
};

struct Const : Expression
{
    Const(Fraction digit)
        : digit(digit){};

    Fraction eval(const std::map<std::string, Fraction> & values = {}) const override;

    Expression * clone() const override;

    ~Const() override = default;

    std::string str() const override;

protected:
    Fraction digit;
};

struct Variable : Expression
{
    Variable(const std::string & name)
        : name(name){};

    Fraction eval(const std::map<std::string, Fraction> & values = {}) const override;

    Expression * clone() const override;

    ~Variable() override = default;

    std::string str() const override;

protected:
    const std::string name;
};

struct Negate : Expression
{
    Negate(const Expression & value)
        : value(value.clone()){};

    Negate(const std::shared_ptr<Expression> & value)
        : value(value)
    {
    }

    Fraction eval(const std::map<std::string, Fraction> & values = {}) const override;

    Expression * clone() const override;

    ~Negate() override = default;

    std::string str() const override;

protected:
    std::shared_ptr<Expression> value;
};

namespace type {
constexpr auto add = std::plus<Fraction>();
constexpr auto subtract = std::minus<Fraction>();
constexpr auto divide = std::divides<Fraction>();
constexpr auto multiply = std::multiplies<Fraction>();
} // namespace type

using Add = AbstractBinary<decltype(type::add), '+'>;
using Subtract = AbstractBinary<decltype(type::subtract), '-'>;
using Multiply = AbstractBinary<decltype(type::multiply), '*'>;
using Divide = AbstractBinary<decltype(type::divide), '/'>;

Add operator+(const Expression & left, const Expression & right);

Subtract operator-(const Expression & left, const Expression & right);

Multiply operator*(const Expression & left, const Expression & right);

Divide operator/(const Expression & left, const Expression & right);

Negate operator-(const Expression & digit);
