//
// Created by stanislavv on 6/15/23.
//
#include "expression.h"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <random>
#include <tuple>

const size_t N = 1e4;

TEST_CASE("Expression CONST")
{
    REQUIRE(Const(0).eval() == 0);
    REQUIRE(Const(17).eval() == 17);

    REQUIRE(Const(1).eval() == 1);
    REQUIRE(Const(19).eval({{"x", 33}}) == 19);
}

TEST_CASE("Expression VAR")
{
    REQUIRE(Variable("x").eval({{"x", 9},
                                {"y", 10}}) == 9);
    Variable var("x");
    for (std::size_t i = 0; i < N; i++) {
        REQUIRE(Add(var, Const(1)).eval({{"x", i}}) == i + 1);
    }
}

TEST_CASE("Expression NEG")
{
    Variable var("x");
    REQUIRE(Negate(var).eval({{"x", 0}}) == -Fraction(0));
    REQUIRE(Negate(var).eval({{"x", 0}}) == Fraction(0));
    REQUIRE(Negate(var).eval({{"x", 10}}) == -10);
    REQUIRE(Negate(var).eval({{"x", -10}}) == 10);
    REQUIRE(-Negate(var).eval({{"x", -10}}) == -10);
    REQUIRE(-(-(var)).eval({{"x", -10}}) == -10);
    REQUIRE(-(-var).eval({{"x", -10}}) == -10);
    REQUIRE(-var.eval({{"x", -10}}) == 10);
    REQUIRE((-var).eval({{"x", -10}}) == 10);
}

TEST_CASE("Expression ADD")
{
    Variable x("x"), y("y");
    for (std::size_t i = 0; i < N; i++) {
        REQUIRE((x + Const(0)).eval({{"x", i}}) == i);
        REQUIRE((y + Const(0)).eval({{"y", i}}) == i);
        REQUIRE((x + -x).eval({{"x", i}}) == 0);
        REQUIRE((y + -y).eval({{"y", i}}) == 0);
    }
    REQUIRE(Add(x, y).eval({{"x", 10},
                            {"y", 11}}) ==
            21);
    REQUIRE((x + y).eval({{"x", 10},
                          {"y", 11}}) ==
            21);
    REQUIRE((-x + y).eval({{"x", 10},
                           {"y", 11}}) ==
            1);
    REQUIRE((x + -y).eval({{"x", 10},
                           {"y", 11}}) ==
            -1);
    REQUIRE((-x + -y).eval({{"x", 10},
                            {"y", 11}}) ==
            -21);
    REQUIRE((-(x + y)).eval({{"x", 10},
                             {"y", 11}}) ==
            (-x + -y).eval({{"x", 10},
                            {"y", 11}}));
}
TEST_CASE("Expression SUB")
{
    Variable x("x"), y("y");
    for (std::size_t i = 0; i < N; i--) {
        REQUIRE((x - Const(0)).eval({{"x", i}}) == i);
        REQUIRE((y - Const(0)).eval({{"y", i}}) == i);
        REQUIRE((x - -x).eval({{"x", i}}) == 0);
        REQUIRE((y - -y).eval({{"y", i}}) == 0);
    }
    REQUIRE(Subtract(x, y).eval({{"x", 10},
                                 {"y", 11}}) ==
            -1);
    REQUIRE((x - y).eval({{"x", 10},
                          {"y", 11}}) ==
            -1);
    REQUIRE((-x - y).eval({{"x", 10},
                           {"y", 11}}) ==
            -21);
    REQUIRE((x - -y).eval({{"x", 10},
                           {"y", 11}}) ==
            21);
    REQUIRE((-x - -y).eval({{"x", 10},
                            {"y", 11}}) ==
            1);
    REQUIRE((-(x - y)).eval({{"x", 10},
                             {"y", 11}}) ==
            (-x - -y).eval({{"x", 10},
                            {"y", 11}}));
}

TEST_CASE("Expression MUL")
{
    Variable x("x"), y("y");
    for (std::size_t i = 0; i < N; i++) {
        REQUIRE((x * Const(0)).eval({{"x", i}}) == 0);
        REQUIRE((y * Const(0)).eval({{"y", i}}) == 0);
        REQUIRE((x * Const(1)).eval({{"x", i}}) == i);
        REQUIRE((y * Const(1)).eval({{"y", i}}) == i);
    }
    REQUIRE(Multiply(x, y).eval({{"x", 10},
                                 {"y", 11}}) ==
            110);
    REQUIRE((x * y).eval({{"x", 10},
                          {"y", 11}}) ==
            110);
    REQUIRE((-x * y).eval({{"x", 10},
                           {"y", 11}}) ==
            -110);
    REQUIRE((x * -y).eval({{"x", 10},
                           {"y", 11}}) ==
            -110);
    REQUIRE((-x * -y).eval({{"x", 10},
                            {"y", 11}}) ==
            110);
    REQUIRE((-(x * y)).eval({{"x", 10},
                             {"y", 11}}) ==
            (-x * y).eval({{"x", 10},
                           {"y", 11}}));
}

TEST_CASE("Expression DIV")
{
    Variable x("x"), y("y");
    for (std::size_t i = 1; i < N; i++) {
        REQUIRE((x / x).eval({{"x", i}}) == 1);
        REQUIRE((y / y).eval({{"y", i}}) == 1);
        REQUIRE((x / Const(1)).eval({{"x", i}}) == i);
        REQUIRE((y / Const(1)).eval({{"y", i}}) == i);
    }
    REQUIRE(Divide(x, y).eval({{"x", 10},
                               {"y", 11}}) ==
            Fraction(10, 11));
    REQUIRE((x / y).eval({{"x", 10},
                          {"y", 11}}) ==
            Fraction(10, 11));
    REQUIRE((-x / y).eval({{"x", 10},
                           {"y", 11}}) ==
            -Fraction(10, 11));
    REQUIRE((x / -y).eval({{"x", 10},
                           {"y", 11}}) ==
            -Fraction(10, 11));
    REQUIRE((-x / -y).eval({{"x", 10},
                            {"y", 11}}) ==
            Fraction(10, 11));
    REQUIRE((-(x / y)).eval({{"x", 10},
                             {"y", 11}}) ==
            (-x / y).eval({{"x", 10},
                           {"y", 11}}));
}
TEST_CASE("Expression STR")
{
    for (std::size_t i = 0; i < N; i++) {
        REQUIRE(Const(i).str() == std::to_string(i) + "/1");
        REQUIRE(Variable(std::to_string(i)).str() == std::to_string(i));
        REQUIRE(Negate(Const(i)).str() == "(-" + std::to_string(i) + "/1)");
        REQUIRE((Negate(Const(i)) + Const(i) + Variable("x")).str() ==
                "(((-" + std::to_string(i) + "/1) + " + std::to_string(i) + "/1) + x)");
        REQUIRE((Negate(Const(i)) - Const(i) - Variable("x")).str() ==
                "(((-" + std::to_string(i) + "/1) - " + std::to_string(i) + "/1) - x)");
        REQUIRE((Negate(Const(i)) * Const(i) * Variable("x")).str() ==
                "(((-" + std::to_string(i) + "/1) * " + std::to_string(i) + "/1) * x)");
        REQUIRE((Negate(Const(i)) / Const(i) / Variable("x")).str() ==
                "(((-" + std::to_string(i) + "/1) / " + std::to_string(i) + "/1) / x)");
    }
}
TEST_CASE("Expression CLN")
{
    for (std::size_t i = 0; i < N; i++) {
        Add x(Const(static_cast<int64_t>(i)), Variable("x"));
        auto y = dynamic_cast<Add *>(x.clone());
        REQUIRE(x.eval({{"x", i}}) == (*y).eval({{"x", i}}));
    }
}
TEST_CASE("Expression EVL")
{
    for (std::size_t i = 0; i < N; ++i) {
        std::mt19937 gen;
        gen.seed(std::time(nullptr));
        int64_t x_val = gen();
        uint64_t y_val = gen();
        Variable x("x"), y("y");
        REQUIRE(Fraction(x_val, y_val) == (x / y).eval({{"x", x_val}, {"y", y_val}}));
        REQUIRE(Fraction(x_val * y_val) == (x * y).eval({{"x", x_val}, {"y", y_val}}));
        REQUIRE(Fraction(x_val + y_val) == (x + y).eval({{"x", x_val}, {"y", y_val}}));
        REQUIRE(Fraction(x_val - y_val) == (x - y).eval({{"x", x_val}, {"y", y_val}}));
        REQUIRE(Fraction(((Fraction(x_val + y_val)) / Fraction(x_val - y_val)) * Fraction(1, 1000)) ==
                (((x + y) / (x - y)) / Const(Fraction(1000))).eval({{"x", x_val}, {"y", y_val}}));
    }
}
TEST_CASE("Fraction CONSTR_REDUCE")
{
    const Fraction zero;
    const Fraction one = 1;
    const Fraction one_neg(1, -1);
    const Fraction four_div_nine(4, 9);
    const Fraction four_div_nine_neg(4, -9);
    const Fraction eleven = 11;
    const Fraction eleven_neg(11, -1);
    REQUIRE(one == 1);
    REQUIRE(-one == -1);
    REQUIRE(one_neg == -1);
    REQUIRE(zero == 0);
    REQUIRE(zero.numerator() == 0);
    REQUIRE(zero.denominator() == 1);
    REQUIRE(-one.numerator() == -1);
    REQUIRE(one_neg.numerator() == -1);
    REQUIRE(one.denominator() == 1);
    REQUIRE(-eleven.numerator() == -11);
    REQUIRE(eleven_neg.numerator() == -11);
    REQUIRE(eleven.denominator() == 1);
    REQUIRE(-four_div_nine.numerator() == -4);
    REQUIRE(four_div_nine_neg.numerator() == -4);
    REQUIRE(four_div_nine.denominator() == 9);
    REQUIRE(Fraction(0, 1121221) == Fraction(0, 1));
    REQUIRE(-Fraction(2, 4).operator double() == -1.0 / 2.0);
    REQUIRE(Fraction(2, 4).operator double() == 1.0 / 2.0);
    for (int i = 1; i < N / 100; ++i) {
        for (int y = N; y > 0; y--) {
            REQUIRE(Fraction(y, i).operator double() == double(y) / double(i));
        }
    }

    REQUIRE(Fraction(2, 4) == Fraction(1, 2));
    REQUIRE(Fraction(0, 20) == Fraction(0));
    for (int i = 1; i < N; ++i) {
        std::mt19937 gen;
        gen.seed(std::time(nullptr));
        int64_t x_val = gen();
        REQUIRE(Fraction(i, x_val) == Fraction(i * 4, x_val * 4));
        REQUIRE(Fraction(i * 2, x_val) == Fraction(i * 4, x_val * 2));
        REQUIRE(Fraction(i, x_val) * 2 == Fraction(i + i, x_val));
    }
}

TEST_CASE("Fraction ADD")
{
    Fraction x(1, 3);
    Fraction y(1, 4);
    REQUIRE(x + 0 == x);
    REQUIRE(x + 1 == Fraction(4, 3));
    REQUIRE(x + y == Fraction(7, 12));
    x += y;
    REQUIRE(x == Fraction(7 * 50, 12 * 50));
    REQUIRE(y == Fraction(1 * 70, 4 * 70));
}

TEST_CASE("Fraction SUB")
{
    Fraction x(1, 3);
    Fraction y(1, 4);
    REQUIRE(x - 0 == x);
    REQUIRE(x - 1 == Fraction(-2, 3));
    REQUIRE(x - y == Fraction(1, 12));
    x -= y;
    REQUIRE(x == Fraction(1 * 50, 12 * 50));
    REQUIRE(y == Fraction(1 * 70, 4 * 70));
}

TEST_CASE("Fraction MUL")
{
    REQUIRE(Fraction(0) * 0 == 0);
    REQUIRE(Fraction(0) * 12 == 0);
    REQUIRE(Fraction(1) * 0 == 0);
    REQUIRE(Fraction(12) * 12 == 144);
    Fraction x(-4, 9);
    REQUIRE(x * 0 == 0);
    REQUIRE(1 * x == x);
    REQUIRE(0 * x == 0);
    REQUIRE(x * 1 == x);
    const Fraction y(9, 10);
    REQUIRE(x * y == Fraction(-2, 5));
    REQUIRE(y * x == Fraction(-2, 5));
    x *= -1;
    REQUIRE(x == Fraction(4, 9));
}

TEST_CASE("Fraction DIV")
{
    REQUIRE(Fraction(0) / 12 == 0);
    REQUIRE(Fraction(12) / 12 == 1);
    Fraction x(-4, 9);
    REQUIRE(x * 0 == 0);
    REQUIRE(x / 1 == x);
    REQUIRE(0 / x == 0);
    REQUIRE(x / 1 == x);
    const Fraction y(9, 10);
    REQUIRE(x / y == Fraction(-40, 81));
    REQUIRE(y / x == Fraction(-81, 40));
    x /= -1;
    REQUIRE(x == Fraction(4, 9));
}

TEST_CASE("Fraction BIG")
{
    REQUIRE(Fraction(1, UINT64_MAX) + Fraction(2, UINT64_MAX) == Fraction(3, UINT64_MAX));
    REQUIRE(Fraction(1, UINT64_MAX) - Fraction(2, UINT64_MAX) == Fraction(-1, UINT64_MAX));
    REQUIRE(Fraction(1, UINT64_MAX) * Fraction(2, UINT64_MAX) == Fraction(2, UINT64_MAX * UINT64_MAX));
    REQUIRE(Fraction(1, UINT64_MAX) / Fraction(2, UINT64_MAX) == Fraction(1, 2));
}

TEST_CASE("Fraction CAST")
{
    REQUIRE(static_cast<double>(Fraction(331)) == 331.0);
    REQUIRE(static_cast<double>(Fraction(-331, 10)) == -33.1);
    REQUIRE(static_cast<double>(Fraction(999, 3)) == 333);
}

TEST_CASE("Fraction EQUALS")
{
    const Fraction y(40, 320);
    const Fraction x(1, 8);
    const Fraction z = x;
    const Fraction other(1323214, 2343905903);
    REQUIRE(x == x);
    REQUIRE(x == y);
    REQUIRE(x == z);
    REQUIRE(x != other);
}

TEST_CASE("Fraction COMPARE")
{
    REQUIRE(Fraction(1, 2) > Fraction(1, 8));
    REQUIRE(Fraction(1, 2) >= Fraction(1, 2));
    REQUIRE(Fraction(2, 7) < Fraction(3, 7));
    REQUIRE(Fraction(2, 7) <= Fraction(2, 7));
}
