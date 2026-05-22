#include "../src/Sort2.h"
#include "catch2/catch_test_macros.hpp"
#include <cstring>
#include <string>

TEST_CASE("Sort2 with integers")
{
    SECTION("Already sorted")
    {
        int a = 1, b = 2;
        Sort2(a, b);
        REQUIRE(a == 1);
        REQUIRE(b == 2);
    }
    
    SECTION("Need swap")
    {
        int a = 5, b = 3;
        Sort2(a, b);
        REQUIRE(a == 3);
        REQUIRE(b == 5);
    }
    
    SECTION("Equal values")
    {
        int a = 7, b = 7;
        Sort2(a, b);
        REQUIRE(a == 7);
        REQUIRE(b == 7);
    }
    
    SECTION("Negative numbers")
    {
        int a = -5, b = -10;
        Sort2(a, b);
        REQUIRE(a == -10);
        REQUIRE(b == -5);
    }
    
    SECTION("Mixed signs")
    {
        int a = -3, b = 2;
        Sort2(a, b);
        REQUIRE(a == -3);
        REQUIRE(b == 2);
    }
}

TEST_CASE("Sort2 with doubles")
{
    SECTION("Already sorted")
    {
        double a = 1.5, b = 2.7;
        Sort2(a, b);
        REQUIRE(a == 1.5);
        REQUIRE(b == 2.7);
    }
    
    SECTION("Need swap")
    {
        double a = 3.14, b = 2.71;
        Sort2(a, b);
        REQUIRE(a == 2.71);
        REQUIRE(b == 3.14);
    }
    
    SECTION("Equal values")
    {
        double a = 5.5, b = 5.5;
        Sort2(a, b);
        REQUIRE(a == 5.5);
        REQUIRE(b == 5.5);
    }
    
    SECTION("Negative doubles")
    {
        double a = -1.1, b = -2.2;
        Sort2(a, b);
        REQUIRE(a == -2.2);
        REQUIRE(b == -1.1);
    }
}

TEST_CASE("Sort2 with std::string")
{
    SECTION("Already sorted")
    {
        std::string a = "alps", b = "topic";
        Sort2(a, b);
        REQUIRE(a == "alps");
        REQUIRE(b == "topic");
    }
    
    SECTION("Need swap")
    {
        std::string a = "zebra", b = "marti";
        Sort2(a, b);
        REQUIRE(a == "marti");
        REQUIRE(b == "zebra");
    }
    
    SECTION("Equal strings")
    {
        std::string a = "hello", b = "hello";
        Sort2(a, b);
        REQUIRE(a == "hello");
        REQUIRE(b == "hello");
    }
    
    SECTION("Case sensitive")
    {
        std::string a = "Apple", b = "apple";
        Sort2(a, b);
        REQUIRE(a == "Apple");
        REQUIRE(b == "apple");
    }
    
    SECTION("Different lengths")
    {
        std::string a = "car", b = "cars";
        Sort2(a, b);
        REQUIRE(a == "car");
        REQUIRE(b == "cars");
    }
    
    SECTION("Empty and non-empty strings")
    {
        std::string a = "", b = "empty string";
        Sort2(a, b);
        REQUIRE(a == "");
        REQUIRE(b == "empty string");
    }
}

TEST_CASE("Sort2 with const char*")
{
    SECTION("Already sorted")
    {
        const char* a = "alps";
        const char* b = "topic";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "alps") == 0);
        REQUIRE(std::strcmp(b, "topic") == 0);
    }
    
    SECTION("Need swap")
    {
        const char* a = "zebra";
        const char* b = "marti";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "marti") == 0);
        REQUIRE(std::strcmp(b, "zebra") == 0);
    }
    
    SECTION("Equal strings")
    {
        const char* a = "world";
        const char* b = "world";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "world") == 0);
        REQUIRE(std::strcmp(b, "world") == 0);
    }
    
    SECTION("Different lengths")
    {
        const char* a = "car";
        const char* b = "cars";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "car") == 0);
        REQUIRE(std::strcmp(b, "cars") == 0);
    }
    
    SECTION("Empty and non-empty")
    {
        const char* a = "";
        const char* b = "hello";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "") == 0);
        REQUIRE(std::strcmp(b, "hello") == 0);
    }
    
    SECTION("Prefix matches")
    {
        const char* a = "hello world";
        const char* b = "hello";
        Sort2(a, b);
        REQUIRE(std::strcmp(a, "hello") == 0);
        REQUIRE(std::strcmp(b, "hello world") == 0);
    }
}

TEST_CASE("Sort2 with crazy examples")
{
    SECTION("Different case")
    {
    	const char* a = "heLLo";
    	const char* b = "WORLd";
    	Sort2(a, b);
    	REQUIRE(std::strcmp(a, "WORLd") == 0);
    	REQUIRE(std::strcmp(b, "heLLo") == 0);
    }
    
    SECTION("Max and min int compare")
    {
        int a = INT32_MAX;
    	int b = a + 1;
        Sort2(a, b);
        REQUIRE(a == INT32_MIN);
        REQUIRE(b == INT32_MAX);
    }
}
