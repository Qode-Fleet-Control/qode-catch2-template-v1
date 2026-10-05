#include "calc/calc.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <stdexcept>
#include <string>
#include <vector>

using Catch::Matchers::Equals;

TEST_CASE("gcd of positive numbers", "[gcd]")
{
    REQUIRE(calc::gcd(12, 18) == 6);
    REQUIRE(calc::gcd(17, 5) == 1);
}

TEST_CASE("gcd handles zero and negative input", "[gcd]")
{
    CHECK(calc::gcd(0, 0) == 0);
    CHECK(calc::gcd(0, 7) == 7);
    CHECK(calc::gcd(-12, 18) == 6);
}

TEST_CASE("fibonacci", "[fibonacci]")
{
    SECTION("known values")
    {
        CHECK(calc::fibonacci(0) == 0);
        CHECK(calc::fibonacci(1) == 1);
        CHECK(calc::fibonacci(10) == 55);
        CHECK(calc::fibonacci(92) == 7540113804746346429LL);
    }
    SECTION("out of range throws")
    {
        CHECK_THROWS_AS(calc::fibonacci(-1), std::out_of_range);
        CHECK_THROWS_AS(calc::fibonacci(93), std::out_of_range);
    }
}

TEST_CASE("is_prime classifies small numbers", "[is_prime]")
{
    // A data-driven test with a Catch2 generator: one run per row.
    auto [n, expected] = GENERATE(table<long long, bool>({
        {-7, false}, {0, false}, {1, false}, {2, true},
        {9, false}, {97, true}, {7919, true}, {7921, false},
    }));
    CAPTURE(n);
    CHECK(calc::is_prime(n) == expected);
}

TEST_CASE("split", "[split]")
{
    const std::vector<std::string> abc{"a", "b", "c"};
    CHECK_THAT(calc::split("a,b,c", ','), Equals(abc));
    CHECK(calc::split("a,,c", ',').size() == 3);
    CHECK(calc::split("", ',') == std::vector<std::string>{""});

    SECTION("a record keeps field order")
    {
        const auto fields = calc::split("id;name;email", ';');
        REQUIRE(fields.size() == 3);
        CHECK(fields.front() == "id");
        CHECK(fields.back() == "email");
    }
}
