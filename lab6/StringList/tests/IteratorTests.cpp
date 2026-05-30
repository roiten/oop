#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <ranges>
#include <vector>
#include "../CStringList.hpp"

TEST_CASE("Iteration visits elements in insertion order")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    list.PushBack("3");

    std::vector<std::string> result;
    for (auto it = list.begin(); it != list.end(); ++it)
        result.push_back(*it);

    REQUIRE(result == std::vector<std::string>{"1", "2", "3"});
}

TEST_CASE("Reverse iteration visits elements in reverse order")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    list.PushBack("3");

    std::vector<std::string> result;
    for (auto & it : std::ranges::reverse_view(list))
        result.push_back(it);

    REQUIRE(result == std::vector<std::string>{"3", "2", "1"});
    CHECK(*list.rbegin() == "3");
    CHECK(*(--list.rend()) == "1");
}

TEST_CASE("Const iterators are equal to non-const for same position")
{
    CStringList list;
    list.PushBack("10");
    const CStringList& constList = list;

    CHECK(*constList.begin() == "10");
    CHECK(constList.cbegin() == constList.begin());
    CHECK(constList.cend() == constList.end());
}

TEST_CASE("Iterator pre/post increment and decrement")
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");

    auto it1 = list.begin();
    auto it2 = list.begin();

    CHECK(it1 == it2);
    it2++;
    CHECK(it1 != it2);

    auto it3 = it2--;
    CHECK(*it3 == "2");
    CHECK(*it2 == "1");

    auto it4 = ++it1;
    CHECK(*it4 == "2");
    CHECK(*it1 == "2");
}

TEST_CASE("Non-const iterator implicitly converts to const")
{
    CStringList list;
    list.PushBack("1");

    CStringList::CIterator<false> it = list.begin();
    CStringList::CIterator<true> cit = it;

    CHECK(*it == *cit);
    CHECK(it == cit);
}

TEST_CASE("begin() on const list returns const iterator")
{
	CStringList list;
	list.PushBack("1");

	const auto& constList = list;
	auto it = constList.begin();

	static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>);
	CHECK(*it == "1");
}


TEST_CASE("Arrow operator accesses string members")
{
    CStringList list;
    list.PushBack("string");

    CHECK(list.begin()->size() == 6);
}

TEST_CASE("Iterators are compatible with STL algorithms")
{
    CStringList list;
    list.PushBack("c");
    list.PushBack("a");
    list.PushBack("b");

    auto it = std::find(list.begin(), list.end(), "a");
    REQUIRE(it != list.end());
    CHECK(*it == "a");
}

TEST_CASE("GetFrontElement and GetBackElement return correct values")
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

TEST_CASE("GetSize and IsEmpty can show list state correctly")
{
	CStringList list;
	CHECK(list.IsEmpty());
	CHECK(list.GetSize() == 0);
	list.PushBack("1");
	CHECK(list.GetSize() == 1);
	list.PushFront("0");
	CHECK(list.GetSize() == 2);
	list.Erase(list.begin());
	CHECK(list.GetSize() == 1);

	CHECK_FALSE(list.IsEmpty());
	list.Clear();
	CHECK(list.IsEmpty());
	CHECK(list.GetSize() == 0);
}
