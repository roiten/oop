#define CATCH_CONFIG_MAIN
#include "../src/FindMaxEx.h"
#include "../src/Sportsmen.h"
#include "catch2/catch_test_macros.hpp"
#include <iostream>
#include <cstring>

struct CompareCString {
    bool operator()(const char* a, const char* b) const {
        return strcmp(a, b) < 0;
    }
};

TEST_CASE("FindMaxEx with strong exception safety")
{
    SECTION("No exception - normal operation")
    {
        std::vector<int> arr = {1, 5, 3, 9, 2};
        int maxValue = 0;

        REQUIRE(FindMaxEx(arr, maxValue, std::less<int>()) == true);
        REQUIRE(maxValue == 9);
    }

    SECTION("Empty array - returns false, no change")
    {
        std::vector<int> arr;
        int maxValue = 1;

        REQUIRE(FindMaxEx(arr, maxValue, std::less<int>()) == false);
        REQUIRE(maxValue == 1);
    }

    SECTION("Single element")
    {
        std::vector<double> arr = {3.14};
        double maxValue = 0.0;

        REQUIRE(FindMaxEx(arr, maxValue, std::less<double>()) == true);
        REQUIRE(maxValue == 3.14);
    }

    SECTION("All equal elements")
    {
        std::vector<std::string> arr = {"same", "same", "same"};
        std::string maxValue;

        REQUIRE(FindMaxEx(arr, maxValue, std::less<std::string>()) == true);
        REQUIRE(maxValue == "same");
    }
}

TEST_CASE("FindMaxEx with const char*")
{
    SECTION("Normal case")
    {
        std::vector<const char*> arr = {"foo", "bar", "qwe"};
        const char* maxValue = nullptr;

        REQUIRE(FindMaxEx(arr, maxValue, CompareCString()) == true);
        REQUIRE(std::strcmp(maxValue, "qwe") == 0);
    }

    SECTION("Empty array")
    {
        std::vector<const char*> arr;
        const char* maxValue = "init";

        REQUIRE(FindMaxEx(arr, maxValue, CompareCString()) == false);
        REQUIRE(std::strcmp(maxValue, "init") == 0);
    }

    SECTION("Strings with different lengths")
    {
        std::vector<const char*> arr = {"cat", "carrot", "car"};
        const char* maxValue = nullptr;

        REQUIRE(FindMaxEx(arr, maxValue, CompareCString()) == true);
        REQUIRE(std::strcmp(maxValue, "cat") == 0);
        std::cout << "Max string: " << maxValue << std::endl;
    }

    SECTION("Empty strings")
    {
        std::vector<const char*> arr = {"", "non-empty", ""};
        const char* maxValue = nullptr;

        REQUIRE(FindMaxEx(arr, maxValue, CompareCString()) == true);
        REQUIRE(std::strcmp(maxValue, "non-empty") == 0);
    }
}

TEST_CASE("FindMaxEx with Sportsmen")
{
    std::vector<Sportsmen> sportsmens = {
        Sportsmen("Иван Иванов", 180, 75),
        Sportsmen("Петр Петров", 195, 82),
        Sportsmen("Сидор Сидоров", 175, 92),
        Sportsmen("Алексей Алексеев", 185, 70),
        Sportsmen("Дмитрий Дмитриев", 190, 88)
    };

    SECTION("Find sportsmen with maximum height")
    {
        Sportsmen tallest;
        bool result = FindMaxEx(sportsmens, tallest, CompareByHeight());

        REQUIRE(result == true);
        REQUIRE(tallest.getHeight() == 195);
        REQUIRE(tallest.getName() == "Петр Петров");
    }

    SECTION("Find sportsmen with maximum weight")
    {
        Sportsmen heaviest;
        bool result = FindMaxEx(sportsmens, heaviest, CompareByWeight());

        REQUIRE(result == true);
        REQUIRE(heaviest.getWeight() == 92);
        REQUIRE(heaviest.getName() == "Сидор Сидоров");
    }

    SECTION("Find max height using lambda")
    {
        Sportsmen tallest;
        auto compareByHeightLambda = [](const Sportsmen& a, const Sportsmen& b) {
            return a.getHeight() < b.getHeight();
        };

        bool result = FindMaxEx(sportsmens, tallest, compareByHeightLambda);

        REQUIRE(result == true);
        REQUIRE(tallest.getHeight() == 195);
    }

    SECTION("Find max weight using lambda")
    {
        Sportsmen heaviest;
        auto compareByWeightLambda = [](const Sportsmen& a, const Sportsmen& b) {
            return a.getWeight() < b.getWeight();
        };

        bool result = FindMaxEx(sportsmens, heaviest, compareByWeightLambda);

        REQUIRE(result == true);
        REQUIRE(heaviest.getWeight() == 92);
    }

    SECTION("Empty array")
    {
        std::vector<Sportsmen> empty;
        Sportsmen result;
        Sportsmen original = result;

        bool found = FindMaxEx(empty, result, CompareByHeight());

        REQUIRE(found == false);
        REQUIRE(result.getName() == original.getName());
        REQUIRE(result.getHeight() == original.getHeight());
        REQUIRE(result.getWeight() == original.getWeight());
    }

    SECTION("Single sportsmen")
    {
        std::vector<Sportsmen> single = {Sportsmen("Леха Лепёхов", 170, 70)};
        Sportsmen result;

        bool found = FindMaxEx(single, result, CompareByHeight());

        REQUIRE(found == true);
        REQUIRE(result.getName() == "Леха Лепёхов");
        REQUIRE(result.getHeight() == 170);
        REQUIRE(result.getWeight() == 70);
    }

    SECTION("All sportsmens have same height")
    {
        std::vector<Sportsmen> sameHeight = {
            Sportsmen("Первый", 180, 75),
            Sportsmen("Второй", 180, 82),
            Sportsmen("Третий", 180, 92)
        };
        Sportsmen result;

        FindMaxEx(sameHeight, result, CompareByHeight());

        REQUIRE(result.getHeight() == 180);

    	REQUIRE((result.getName() == "Первый" ||
                 result.getName() == "Второй" ||
                 result.getName() == "Третий"));
    }

    SECTION("All sportsmens have same weight")
    {
        std::vector<Sportsmen> sameWeight = {
            Sportsmen("Первый", 180, 80),
            Sportsmen("Второй", 190, 80),
            Sportsmen("Третий", 175, 80)
        };
        Sportsmen result;

        FindMaxEx(sameWeight, result, CompareByWeight());

        REQUIRE(result.getWeight() == 80);
    }

    SECTION("Sportsmens with negative values")
    {
        std::vector<Sportsmen> invalid = {
            Sportsmen("Отрицательный", -10, -5),
            Sportsmen("Положительный", 180, 75)
        };
        Sportsmen result;

        FindMaxEx(invalid, result, CompareByHeight());

        REQUIRE(result.getHeight() == 180);
        REQUIRE(result.getName() == "Положительный");
    }
}
