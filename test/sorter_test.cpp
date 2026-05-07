#include "util.h"
#include "Sorter.h"

#include <string>
#include <vector>

static std::vector<std::string_view> sv(std::initializer_list<const char*> items)
{
    return std::vector<std::string_view>(items.begin(), items.end());
}

TEST(SorterTest, CaseSensitiveAscending)
{
    CaseSensitiveSorter s(Sorter::Direction::Ascending);
    auto v = sv({"banana", "Apple", "cherry"});
    s.sort(v);
    // ASCII: 'A'(65) < 'b'(98) < 'c'(99)
    EXPECT_EQ(v[0], "Apple");
    EXPECT_EQ(v[1], "banana");
    EXPECT_EQ(v[2], "cherry");
}

TEST(SorterTest, CaseSensitiveDescending)
{
    CaseSensitiveSorter s(Sorter::Direction::Descending);
    auto v = sv({"banana", "Apple", "cherry"});
    s.sort(v);
    EXPECT_EQ(v[0], "cherry");
    EXPECT_EQ(v[1], "banana");
    EXPECT_EQ(v[2], "Apple");
}

TEST(SorterTest, CaseInsensitiveAscending)
{
    CaseInsensitiveSorter s(Sorter::Direction::Ascending);
    auto v = sv({"BANANA", "apple", "Cherry"});
    s.sort(v);
    EXPECT_EQ(v[0], "apple");
    EXPECT_EQ(v[1], "BANANA");
    EXPECT_EQ(v[2], "Cherry");
}

TEST(SorterTest, CaseInsensitiveDescending)
{
    CaseInsensitiveSorter s(Sorter::Direction::Descending);
    auto v = sv({"BANANA", "apple", "Cherry"});
    s.sort(v);
    EXPECT_EQ(v[0], "Cherry");
    EXPECT_EQ(v[1], "BANANA");
    EXPECT_EQ(v[2], "apple");
}

TEST(SorterTest, LineLengthAscending)
{
    LineLengthSorter s(Sorter::Direction::Ascending);
    auto v = sv({"cc", "aaaa", "b"});
    s.sort(v);
    EXPECT_EQ(v[0], "b");
    EXPECT_EQ(v[1], "cc");
    EXPECT_EQ(v[2], "aaaa");
}

TEST(SorterTest, LineLengthDescending)
{
    LineLengthSorter s(Sorter::Direction::Descending);
    auto v = sv({"cc", "aaaa", "b"});
    s.sort(v);
    EXPECT_EQ(v[0], "aaaa");
    EXPECT_EQ(v[1], "cc");
    EXPECT_EQ(v[2], "b");
}

TEST(SorterTest, ReverseSort)
{
    ReverseSorter s(Sorter::Direction::Ascending);
    auto v = sv({"a", "b", "c"});
    s.sort(v);
    EXPECT_EQ(v[0], "c");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "a");
    // Applying twice returns original order
    s.sort(v);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
    EXPECT_EQ(v[2], "c");
}

TEST(SorterTest, EmptyVector)
{
    CaseSensitiveSorter s(Sorter::Direction::Ascending);
    std::vector<std::string_view> v;
    EXPECT_NO_THROW(s.sort(v));
    EXPECT_TRUE(v.empty());
}

TEST(SorterTest, SingleElement)
{
    CaseSensitiveSorter s(Sorter::Direction::Ascending);
    auto v = sv({"hello"});
    s.sort(v);
    EXPECT_EQ(v[0], "hello");
}
