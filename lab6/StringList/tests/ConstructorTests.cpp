#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../CStringList.hpp"

TEST_CASE("Default constructor creates empty list", "[constructor]")
{
    CStringList list;
    CHECK(list.IsEmpty());
    CHECK(list.GetSize() == 0);
}

TEST_CASE("Copy constructor produces independent deep copy", "[constructor]")
{
    CStringList original;
    original.PushBack("hello");
    original.PushBack("world");

    CStringList copy(original);

    REQUIRE(copy.GetSize() == 2);
    CHECK(copy.GetFrontElement() == "hello");
    CHECK(copy.GetBackElement() == "world");

    *copy.begin() = "modified";
    CHECK(original.GetFrontElement() == "hello");
}

TEST_CASE("Move constructor transfers ownership and empties source", "[constructor]")
{
    CStringList source;
    source.PushBack("move_me");

    CStringList dest(std::move(source));

    CHECK(dest.GetSize() == 1);
    CHECK(dest.GetFrontElement() == "move_me");
    CHECK(source.IsEmpty());
}

TEST_CASE("Copy assignment operator", "[constructor]")
{
    CStringList list1;
    list1.PushBack("A");

    CStringList list2;
    list2.PushBack("B");

    list1 = list2;
    CHECK(list1.GetSize() == 1);
    CHECK(list1.GetFrontElement() == "B");

    list1 = *&list1;
    CHECK(list1.GetFrontElement() == "B");
}

TEST_CASE("Move assignment operator", "[constructor]")
{
    CStringList list1;

    CStringList list2;
    list2.PushBack("temporary");

    list1 = std::move(list2);
    CHECK(list1.GetSize() == 1);
    CHECK(list1.GetFrontElement() == "temporary");
    CHECK(list2.IsEmpty());
}
