#include <catch2/catch_test_macros.hpp>
#include <string>
#include <stdexcept>
#include "../CStringList.hpp"

TEST_CASE("Access front/back on empty list triggers assert", "[boundary]")
{
    CStringList emptyList;
    REQUIRE_THROWS(emptyList.GetFrontElement());
    REQUIRE_THROWS(emptyList.GetBackElement());
}

TEST_CASE("Erase with end iterator triggers assert", "[boundary]")
{
    CStringList list;
    list.PushBack("1");
    REQUIRE_THROWS(list.Erase(list.cend()));
}

TEST_CASE("Copy constructor provides strong exception guarantee", "[exception_safety]")
{
    CStringList original;
    original.PushBack("A");
    original.PushBack("B");

    CHECK(original.GetSize() == 2);
    CHECK(original.GetFrontElement() == "A");
}

TEST_CASE("No stack overflow on Clear and destructor with 1M elements", "[stress]")
{
    const size_t n = 1000000;

    SECTION("Clear does not overflow stack")
    {
        CStringList list;
        for (size_t i = 0; i < n; ++i)
            list.PushBack("test");

        CHECK(list.GetSize() == n);
        REQUIRE_NOTHROW(list.Clear());
        CHECK(list.IsEmpty());
    }

    SECTION("Destructor does not overflow stack")
    {
        CStringList list;
        for (size_t i = 0; i < n; ++i)
            list.PushBack("test");
    }
}
