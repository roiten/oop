#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../CStringList.hpp"

TEST_CASE("Front and back element access", "[access]")
{
    CStringList list;
    list.PushBack("start");
    list.PushBack("middle");
    list.PushBack("finish");

    CHECK(list.GetFrontElement() == "start");
    CHECK(list.GetBackElement() == "finish");

    *list.begin() = "first";
    CHECK(list.GetFrontElement() == "first");

    const CStringList& constList = list;
    CHECK(constList.GetFrontElement() == "first");
    CHECK(constList.GetBackElement() == "finish");
}

TEST_CASE("Size and empty state", "[access]")
{
    CStringList list;
    CHECK(list.IsEmpty());
    CHECK(list.GetSize() == 0);

    list.PushBack("1");
    CHECK_FALSE(list.IsEmpty());
    CHECK(list.GetSize() == 1);

    list.PushFront("0");
    CHECK(list.GetSize() == 2);

    list.Erase(list.begin());
    CHECK(list.GetSize() == 1);

    list.Clear();
    CHECK(list.IsEmpty());
    CHECK(list.GetSize() == 0u);
}
