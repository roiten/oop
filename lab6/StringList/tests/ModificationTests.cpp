#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../CStringList.hpp"

TEST_CASE("PushBack and PushFront add elements to correct ends")
{
    CStringList list;
    list.PushBack("second");
    list.PushFront("first");

    CHECK(list.GetSize() == 2);
    CHECK(list.GetFrontElement() == "first");
    CHECK(list.GetBackElement() == "second");
}

TEST_CASE("Emplace inserts at different position")
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

TEST_CASE("Erase removes element and returns iterator to next")
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

    list.Erase(list.begin());
	CHECK(list.GetFrontElement() == "3");

    list.Erase(list.begin());
    CHECK(list.IsEmpty());
}

TEST_CASE("PopFront removes first element")
{
    CStringList list;
    list.PushBack("first");
    list.PushBack("second");

    list.PopFront();

    CHECK(list.GetSize() == 1);
    CHECK(list.GetFrontElement() == "second");
}

TEST_CASE("PopBack removes last element")
{
    CStringList list;
    list.PushBack("first");
    list.PushBack("second");

    list.PopBack();

    CHECK(list.GetSize() == 1);
    CHECK(list.GetBackElement() == "first");
}

TEST_CASE("Clear empties the list")
{
    SECTION("on empty list")
    {
        CStringList list;
        list.Clear();
        CHECK(list.IsEmpty());
    }

    SECTION("on non-empty list")
    {
        CStringList list;
        list.PushBack("1");
        list.PushBack("2");
        list.Clear();

        CHECK(list.IsEmpty());
        CHECK(list.GetSize() == 0);
        CHECK(list.begin() == list.end());
    }
}

TEST_CASE("List can be modified through iterator")
{
    CStringList list;
    list.PushBack("old");

    *list.begin() = "new";

    CHECK(list.GetFrontElement() == "new");
	CHECK(list.GetSize() == 1);
}

TEST_CASE("Range-based for loop iterates all elements")
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    std::string result;
    for (const auto& s : list)
        result += s;

    CHECK(result == "abc");
}

TEST_CASE("Large strings are stored and moved correctly")
{
    CStringList list;
    std::string large(1000, 'q');

    list.PushBack(large);
    list.PushBack(std::move(large));

    CHECK(list.GetSize() == 2);
    CHECK(list.GetFrontElement().length() == 1000);
    CHECK(list.GetBackElement().length() == 1000);
}
