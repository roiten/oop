#include <catch2/catch_test_macros.hpp>
#include <string>
#include <type_traits>
#include "../CStringList.hpp"

TEST_CASE("CIterator<false> iterator_traits are correct", "[member_types]")
{
    using Iter = CStringList::CIterator<false>;

    static_assert(std::is_same_v<
        std::iterator_traits<Iter>::iterator_category,
        std::bidirectional_iterator_tag>);

    static_assert(std::is_same_v<
        std::iterator_traits<Iter>::value_type,
        std::string>);

    static_assert(std::is_same_v<
        std::iterator_traits<Iter>::pointer,
        std::string*>);

    static_assert(std::is_same_v<
        std::iterator_traits<Iter>::reference,
        std::string&>);

    SUCCEED();
}

TEST_CASE("CIterator<true> iterator_traits are const-correct", "[member_types]")
{
    using CIter = CStringList::CIterator<true>;

    static_assert(std::is_same_v<
        std::iterator_traits<CIter>::pointer,
        const std::string*>);

    static_assert(std::is_same_v<
        std::iterator_traits<CIter>::reference,
        const std::string&>);

    SUCCEED();
}

TEST_CASE("Reverse iterator types match expected", "[member_types]")
{
    using RIter  = std::reverse_iterator<CStringList::CIterator<false>>;
    using CRIter = std::reverse_iterator<CStringList::CIterator<true>>;

    static_assert(std::is_same_v<
        decltype(std::declval<CStringList>().rbegin()), RIter>);

    static_assert(std::is_same_v<
        decltype(std::declval<const CStringList>().rbegin()), CRIter>);

    SUCCEED();
}
