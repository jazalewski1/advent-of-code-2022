#include "Task.hpp"
#include "utility/Dbg.hpp"
#include <gtest/gtest.h>

TEST(HasRedundantRangeTest, No_overlap)
{
    EXPECT_FALSE(task::has_redundant_range("1-4,6-9"));
}

TEST(HasRedundantRangeTest, Overlap_but_not_contained)
{
    EXPECT_FALSE(task::has_redundant_range("1-6,4-9"));
    EXPECT_FALSE(task::has_redundant_range("7-53,21-99"));
}

TEST(HasRedundantRangeTest, Contain_without_touching)
{
    EXPECT_TRUE(task::has_redundant_range("1-9,4-6"));
    EXPECT_TRUE(task::has_redundant_range("4-6,1-9"));
    EXPECT_TRUE(task::has_redundant_range("1-9,6-6"));
    EXPECT_TRUE(task::has_redundant_range("6-6,1-9"));
}

TEST(HasRedundantRangeTest, Contain_with_touching)
{
    EXPECT_TRUE(task::has_redundant_range("1-9,4-9"));
    EXPECT_TRUE(task::has_redundant_range("4-9,1-9"));
    EXPECT_TRUE(task::has_redundant_range("9-9,1-9"));
    EXPECT_TRUE(task::has_redundant_range("1-9,1-1"));
    EXPECT_TRUE(task::has_redundant_range("1-9,1-9"));
}

namespace
{
    using task::Range;
void expect_overlapping(const Range& r1, const Range& r2)
{
    EXPECT_TRUE(task::are_overlapping(r1, r2));
    EXPECT_TRUE(task::are_overlapping(r2, r1));
}
void expect_not_overlapping(const Range& r1, const Range& r2)
{
    EXPECT_FALSE(task::are_overlapping(r1, r2));
    EXPECT_FALSE(task::are_overlapping(r2, r1));
}
} // namespace

TEST(AreOverlappingTest, When_ends_touch)
{
    expect_overlapping(Range{1, 8}, Range{8, 12});
}

TEST(AreOverlappingTest, When_part_overlaps)
{
    expect_overlapping(Range{1, 8}, Range{6, 12});
}

TEST(AreOverlappingTest, When_one_fully_contains_the_other)
{
    expect_overlapping(Range{1, 9}, Range{4, 6});
    expect_overlapping(Range{1, 9}, Range{1, 6});
    expect_overlapping(Range{1, 9}, Range{4, 9});
    expect_overlapping(Range{1, 9}, Range{4, 4});
    expect_overlapping(Range{1, 9}, Range{1, 1});
    expect_overlapping(Range{1, 9}, Range{9, 9});
}

TEST(AreOverlappingTest, When_they_are_equal)
{
    expect_overlapping(Range{4, 8}, Range{4, 8});
}

TEST(AreOverlappingTest, Are_not_overlapping)
{
    expect_not_overlapping(Range{1, 5}, Range{9, 12});
    expect_not_overlapping(Range{4, 4}, Range{5, 5});
}
