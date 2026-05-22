#define CATCH_CONFIG_MAIN
#include "../src/FindMax.h"
#include "catch2/catch_test_macros.hpp"

#include <iostream>

struct ThrowingOnCopy {
    int value;
    static bool shouldThrow;
    
    ThrowingOnCopy(int v = 0) : value(v) {}
    
    ThrowingOnCopy(const ThrowingOnCopy& other) : value(other.value) {
        if (shouldThrow) {
            throw std::runtime_error("Copy failed!");
        }
    }
    
	auto operator<=>(const ThrowingOnCopy& other) const {
    	return value <=> other.value;
    }
};

bool ThrowingOnCopy::shouldThrow = false;

TEST_CASE("FindMax with strong exception safety")
{
    SECTION("No exception - normal operation")
    {
        std::vector<int> arr = {1, 5, 3, 9, 2};
        int maxValue = 0;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(maxValue == 9);
    }
    
    SECTION("Empty array - returns false, no change")
    {
        std::vector<int> arr;
        int maxValue = 1;
        
        REQUIRE(FindMax(arr, maxValue) == false);
        REQUIRE(maxValue == 1);
    }
    
    SECTION("Single element")
    {
        std::vector<double> arr = {3.14};
        double maxValue = 0.0;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(maxValue == 3.14);
    }
    
    SECTION("All equal elements")
    {
        std::vector<std::string> arr = {"same", "same", "same"};
        std::string maxValue;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(maxValue == "same");
    }
}

TEST_CASE("FindMax with const char*")
{
    SECTION("Normal case")
    {
        std::vector<const char*> arr = {"foo", "bar", "qwe"};
        const char* maxValue = nullptr;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(std::strcmp(maxValue, "qwe") == 0);
    }
    
    SECTION("Empty array")
    {
        std::vector<const char*> arr;
        const char* maxValue = "init";
        
        REQUIRE(FindMax(arr, maxValue) == false);
        REQUIRE(std::strcmp(maxValue, "init") == 0);
    }
    
    SECTION("Strings with different lengths")
    {
        std::vector<const char*> arr = {"cat", "carrot", "car"};
        const char* maxValue = nullptr;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(std::strcmp(maxValue, "cat") == 0);
    	std::cout << maxValue << std::endl;
    }
    
    SECTION("Empty strings")
    {
        std::vector<const char*> arr = {"", "non-empty", ""};
        const char* maxValue = nullptr;
        
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(std::strcmp(maxValue, "non-empty") == 0);
    }
}

TEST_CASE("Strong exception safety tests")
{
    SECTION("No change on exception")
    {
        std::vector<ThrowingOnCopy> arr = {ThrowingOnCopy(1), ThrowingOnCopy(5), ThrowingOnCopy(3)};
        ThrowingOnCopy maxValue(999);
        
        ThrowingOnCopy::shouldThrow = true;
        
        REQUIRE_THROWS_AS(FindMax(arr, maxValue), std::runtime_error);
        
        REQUIRE(maxValue.value == 999);
    }
    
    SECTION("Normal operation after exception flag reset")
    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        int maxValue = 0;
        
        ThrowingOnCopy::shouldThrow = false;
        REQUIRE(FindMax(arr, maxValue) == true);
        REQUIRE(maxValue == 5);
    }
}