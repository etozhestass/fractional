//
// Created by stanislavv on 6/15/23.
//

#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ostream>

struct Fraction
{
    template <class T>
    explicit Fraction(int64_t numerator, T denominator)
        : num(numerator * sign(denominator))
        , denom(denominator * sign(denominator))
    {
        reduce();
    }

    Fraction(int64_t numerator)
        : num(numerator)
        , denom(1)
    {
    }

    Fraction()
        : num(0)
        , denom(1)
    {
    }

    [[nodiscard]] int64_t numerator() const;

    [[nodiscard]] uint64_t denominator() const;

    [[nodiscard]] std::string str() const;

    explicit operator double() const
    {
        return static_cast<double>(num) / static_cast<double>(denom);
    };

    friend bool operator>(const Fraction & a, const Fraction & b)
    {
        return static_cast<double>(a) > static_cast<double>(b);
    }

    friend bool operator<(const Fraction & a, const Fraction & b)
    {
        return b > a;
    }

    friend bool operator>=(const Fraction & a, const Fraction & b)
    {
        return !(a < b);
    }

    friend bool operator<=(const Fraction & a, const Fraction & b)
    {
        return !(a > b);
    }

    friend bool operator==(const Fraction & a, const Fraction & b)
    {
        auto res = a.get_gcd_fraction(b);
        return res.first.numerator() * static_cast<int64_t>(res.second.denominator()) == res.second.numerator() * static_cast<int64_t>(res.first.denominator());
    };

    friend bool operator!=(const Fraction & a, const Fraction & b)
    {
        return !(a == b);
    }

    std::pair<Fraction, Fraction> get_gcd_fraction(const Fraction & other) const;

private:
    std::int64_t num;
    std::uint64_t denom;

    void reduce();

    template <class T>
    static T sign(T num)
    {
        return std::signbit(num) ? -1 : 1;
    }
};

Fraction operator+(const Fraction & a, const Fraction & b);

void operator+=(Fraction & a, const Fraction & b);

Fraction operator*(const Fraction & a, const Fraction & b);

void operator*=(Fraction & a, const Fraction & b);

Fraction operator/(const Fraction & a, const Fraction & b);

void operator/=(Fraction & a, const Fraction & b);

Fraction operator-(const Fraction & a, const Fraction & b);

Fraction operator-(const Fraction & a);

void operator-=(Fraction & a, const Fraction & b);

std::ostream & operator<<(std::ostream & os, Fraction a);
