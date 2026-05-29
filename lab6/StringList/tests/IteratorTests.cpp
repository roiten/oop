#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>
#include "../CStringList.hpp"

TEST_CASE("Forward iteration", "[iterator]")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    list.PushBack("3");

    std::vector<std::string> result;
    for (auto it = list.begin(); it != list.end(); ++it)
        result.push_back(*it);

    REQUIRE(result == std::vector<std::string>{"1", "2", "3"});

    auto it = list.begin();
    CHECK(*it == "1");
    CHECK(*(++it) == "2");
}

TEST_CASE("Const iteration", "[iterator]")
{
    CStringList list;
    list.PushBack("10");
    const CStringList& constList = list;

    auto it = constList.begin();
    CHECK(*it == "10");
    CHECK(constList.cbegin() == constList.begin());
    CHECK(constList.cend() == constList.end());
}

TEST_CASE("Reverse iteration", "[iterator]")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    list.PushBack("3");

    std::vector<std::string> result;
    for (auto it = list.rbegin(); it != list.rend(); ++it)
        result.push_back(*it);

    REQUIRE(result == std::vector<std::string>{"3", "2", "1"});
    CHECK(*list.rbegin() == "3");
    CHECK(*(--list.rend()) == "1");
}

TEST_CASE("Iterator operators", "[iterator]")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");

    auto it1 = list.begin();
    auto it2 = list.begin();

    CHECK(it1 == it2);
    CHECK_FALSE(it1 != it2);

    it2++;
    CHECK(it1 != it2);

    auto it3 = it2--;
    CHECK(*it3 == "2");
    CHECK(*it2 == "1");

    auto it4 = ++it1;
    CHECK(*it4 == "2");
    CHECK(*it1 == "2");
}

TEST_CASE("Iterator conversion from non-const to const", "[iterator]")
{
    CStringList list;
    list.PushBack("1");

    CStringList::CIterator<false> it = list.begin();
    CStringList::CIterator<true> cit = it;

    CHECK(*it == *cit);
    CHECK(it == cit);
}

TEST_CASE("Arrow operator on iterator", "[iterator]")
{
    CStringList list;
    list.PushBack("hello");

    CHECK(list.begin()->size() == 5);
}

TEST_CASE("STL algorithm compatibility", "[iterator]")
{
    CStringList list;
    list.PushBack("c");
    list.PushBack("a");
    list.PushBack("b");

    auto it = std::find(list.begin(), list.end(), "a");
    REQUIRE(it != list.end());
    CHECK(*it == "a");
}
