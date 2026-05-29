#include <catch2/catch_test_macros.hpp>
#include <string>
#include "../CStringList.hpp"

TEST_CASE("Large string objects are stored correctly", "[additional_types]")
{
    CStringList list;
    std::string large(1000, 'a');

    list.PushBack(large);
    list.PushBack(std::move(large));

    CHECK(list.GetSize() == 2u);
    CHECK(list.GetFrontElement().length() == 1000u);
    CHECK(list.GetBackElement().length() == 1000u);
}

TEST_CASE("Const correctness: begin() on const list returns const iterator", "[additional_types]")
{
    CStringList list;
    list.PushBack("1");

    const auto& clist = list;
    auto it = clist.begin();

    static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>);
    CHECK(*it == "1");
}

TEST_CASE("Modifying elements through iterator", "[additional_types]")
{
    // Given
    CStringList list;
    list.PushBack("old");

    // When
    *list.begin() = "new";

    // Then
    CHECK(list.GetFrontElement() == "new");
}

TEST_CASE("Range-based for loop works correctly", "[additional_types]")
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
