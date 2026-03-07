#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../src/PrimeGen.h"
#include <cstdint>
#include <set>
#include <stdexcept>


TEST_CASE("GeneratePrimeNumbersSet обрабатывает 0")
{
    auto result = GeneratePrimeNumbersSet(0);
    REQUIRE(result.size() == 0);
    REQUIRE(result.empty());
}

TEST_CASE("GeneratePrimeNumbersSet обрабатывает 1")
{
    auto result = GeneratePrimeNumbersSet(1);
    REQUIRE(result.size() == 0);
    REQUIRE(result.empty());
}

TEST_CASE("GeneratePrimeNumbersSet обрабатывает 2")
{
    auto result = GeneratePrimeNumbersSet(2);
    REQUIRE(result.size() == 1);
    REQUIRE(result.find(2) != result.end());
}

TEST_CASE("GeneratePrimeNumbersSet обрабатывает отрицательное число")
{
    uint64_t negative = static_cast<uint64_t>(-5);
    CHECK_THROWS_AS(GeneratePrimeNumbersSet(negative), std::overflow_error);
}

TEST_CASE("GeneratePrimeNumbersSet обрабатывает максимальное отрицательное число")
{
    uint64_t negative_one = static_cast<uint64_t>(-1);
    CHECK_THROWS_AS(GeneratePrimeNumbersSet(negative_one), std::overflow_error);
}

TEST_CASE("GeneratePrimeNumbersSet находит первые простые числа")
{
    SECTION("Простые числа до 18")
    {
        auto result = GeneratePrimeNumbersSet(18);
        std::set<uint64_t> expected = {2, 3, 5, 7, 11, 13, 17};
        REQUIRE(result == expected);
    }

    SECTION("Простые числа до 30")
    {
        auto result = GeneratePrimeNumbersSet(30);
        std::set<uint64_t> expected = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        REQUIRE(result == expected);
    }
}

TEST_CASE("GeneratePrimeNumbersSet даёт 25 простых чисел до 100")
{
	auto result = GeneratePrimeNumbersSet(100);
	REQUIRE(result.size() == 25);
}

TEST_CASE("GeneratePrimeNumbersSet проверяет конкретные числа")
{
    auto result = GeneratePrimeNumbersSet(50);

    REQUIRE(result.find(2) != result.end());
    REQUIRE(result.find(3) != result.end());
    REQUIRE(result.find(5) != result.end());
    REQUIRE(result.find(7) != result.end());
    REQUIRE(result.find(11) != result.end());
    REQUIRE(result.find(13) != result.end());
    REQUIRE(result.find(17) != result.end());
    REQUIRE(result.find(19) != result.end());
    REQUIRE(result.find(23) != result.end());
    REQUIRE(result.find(29) != result.end());
    REQUIRE(result.find(31) != result.end());
    REQUIRE(result.find(37) != result.end());
    REQUIRE(result.find(41) != result.end());
    REQUIRE(result.find(43) != result.end());
    REQUIRE(result.find(47) != result.end());

    REQUIRE(result.find(8) == result.end());
    REQUIRE(result.find(10) == result.end());
}