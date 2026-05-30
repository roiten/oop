#include <catch2/catch_test_macros.hpp>
#include <string>
#include <stdexcept>
#include "../CStringList.hpp"

TEST_CASE("GetFrontElement and GetBackElement throw on empty list")
{
    CStringList emptyList;
    REQUIRE_THROWS(emptyList.GetFrontElement());
    REQUIRE_THROWS(emptyList.GetBackElement());
}

TEST_CASE("Erase throws when called with end iterator")
{
    CStringList list;
    list.PushBack("1");
    REQUIRE_THROWS(list.Erase(list.cend()));
}

TEST_CASE("No stack overflow on Clear and destructor with 1M elements")
{
    const size_t HUGE_SIZE = 1'000'000;

    SECTION("Clear does not overflow stack")
    {
        CStringList list;
        for (size_t i = 0; i < HUGE_SIZE; ++i)
            list.PushBack("test");

        CHECK(list.GetSize() == HUGE_SIZE);
        REQUIRE_NOTHROW(list.Clear());
        CHECK(list.IsEmpty());
    }

    SECTION("Destructor does not overflow stack")
    {
        CStringList list;
        for (size_t i = 0; i < HUGE_SIZE; ++i)
            list.PushBack("test");
    }
}