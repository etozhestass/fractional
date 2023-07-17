//
// Created by stanislavv on 6/15/23.
//
#include "fractional.h"

#include <numeric>
#include <string>

void Fraction::reduce()
{
    if (!denominator()) {
        num = num ? sign(num) : 0;
        return;
    }
    uint64_t gcd = std::gcd(std::abs(num), denom);
    num /= static_cast<int64_t>(gcd);
    denom /= gcd;
}

int64_t Fraction::numerator() const
{
    return num;
}

uint64_t Fraction::denominator() const
{
    return denom;
}

std::string Fraction::str() const
{
    return std::to_string(numerator()) + '/' + std::to_string(denominator());
}
std::pair<Fraction, Fraction> Fraction::get_gcd_fraction(const Fraction & other) const
{
    if (!numerator()) {
        return {0, other};
    }
    if (!other.numerator()) {
        return {*this, 0};
    }

    uint64_t gcd_a_b = std::gcd(numerator(), other.denominator());
    uint64_t gcd_b_a = std::gcd(numerator(), denominator());

    uint64_t a_denom_final = denominator() / gcd_b_a;
    uint64_t b_denom_final = other.denominator() / gcd_a_b;

    int64_t a_num_final = numerator() / static_cast<int64_t>(gcd_a_b);
    int64_t b_num_final = other.numerator() / static_cast<int64_t>(gcd_b_a);
    return std::pair<Fraction, Fraction>(Fraction(a_num_final, a_denom_final), Fraction(b_num_final, b_denom_final));
}

Fraction operator+(const Fraction & a, const Fraction & b)
{
    auto res = a.get_gcd_fraction(b);
    return Fraction(
            res.first.numerator() * static_cast<int64_t>(res.second.denominator()) + res.second.numerator() * static_cast<int64_t>(res.first.denominator()),
            res.first.denominator() * res.second.denominator());
}

Fraction operator-(const Fraction & a, const Fraction & b)
{
    return a + (-b);
}

Fraction operator-(const Fraction & a)
{
    return Fraction(-a.numerator(), a.denominator());
}

void operator+=(Fraction & a, const Fraction & b)
{
    a = a + b;
}

void operator-=(Fraction & a, const Fraction & b)
{
    a = a - b;
}

Fraction operator*(const Fraction & a, const Fraction & b)
{
    uint64_t gcd_a_b = std::gcd(a.numerator(), b.denominator());
    uint64_t gcd_b_a = std::gcd(b.numerator(), a.denominator());
    return Fraction((a.numerator() / static_cast<int64_t>(gcd_a_b)) *
                            (b.numerator() / static_cast<int64_t>(gcd_b_a)),
                    (a.denominator() / gcd_b_a) *
                            (b.denominator() / gcd_a_b));
}

Fraction operator/(const Fraction & a, const Fraction & b)
{
    return a * Fraction(static_cast<std::int64_t>(b.denominator()), b.numerator());
}

void operator*=(Fraction & a, const Fraction & b)
{
    a = a * b;
}

void operator/=(Fraction & a, const Fraction & b)
{
    a = a / b;
}

std::ostream & operator<<(std::ostream & os, Fraction a)
{
    return os << a.numerator() << "/" << a.denominator();
}
