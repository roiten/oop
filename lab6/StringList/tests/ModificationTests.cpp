#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../CStringList.hpp"

TEST_CASE("PushBack appends to end", "[modification]")
{
    CStringList list;
    list.PushBack("first");
    list.PushBack("second");

    CHECK(list.GetSize() == 2);
    CHECK(list.GetFrontElement() == "first");
    CHECK(list.GetBackElement() == "second");
}

TEST_CASE("PushFront prepends to beginning", "[modification]")
{
    CStringList list;
    list.PushFront("tail");
    list.PushFront("head");

    CHECK(list.GetSize() == 2);
    CHECK(list.GetFrontElement() == "head");
    CHECK(list.GetBackElement() == "tail");
}

TEST_CASE("Emplace inserts at given iterator position", "[modification]")
{
    CStringList list;
    auto it = list.Emplace(list.cbegin(), "100");
    CHECK(*it == "100");

    list.Emplace(list.cend(), "300");
    list.Emplace(++list.begin(), "200");

    auto checkIt = list.begin();
    CHECK(*checkIt++ == "100");
    CHECK(*checkIt++ == "200");
    CHECK(*checkIt++ == "300");
}

TEST_CASE("Erase removes element at iterator position", "[modification]")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    list.PushBack("3");

    auto it = list.Erase(++list.begin());
    CHECK(list.GetSize() == 2);
    CHECK(*it == "3");
    CHECK(list.GetFrontElement() == "1");
    CHECK(list.GetBackElement() == "3");
	CHECK(list.GetFrontElement() == "3");

    list.Erase(list.begin());
    CHECK(list.IsEmpty());
}

TEST_CASE("Clear removes all elements", "[modification]")
{
    SECTION("Clear on empty list is safe")
    {
        CStringList list;
        list.Clear();
        CHECK(list.IsEmpty());
    }

    SECTION("Clear on non-empty list")
    {
        CStringList list;
        list.PushBack("1");
        list.PushBack("2");
        list.Clear();

        CHECK(list.IsEmpty());
        CHECK(list.GetSize() == 0u);
        CHECK(list.begin() == list.end());
    }
}

TEST_CASE("PopFront removes first element", "[modification]")
{
    CStringList list;
    list.PushBack("first");
    list.PushBack("second");
    list.PopFront();

    CHECK(list.GetSize() == 1);
    CHECK(list.GetFrontElement() == "second");
}

TEST_CASE("PopBack removes last element", "[modification]")
{
    CStringList list;
    list.PushBack("first");
    list.PushBack("second");

    list.PopBack();

    CHECK(list.GetSize() == 1);
    CHECK(list.GetBackElement() == "first");
}
