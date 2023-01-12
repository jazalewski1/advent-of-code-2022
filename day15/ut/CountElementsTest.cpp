#include <gtest/gtest.h>
#include "Task.hpp"

using namespace task;

TEST(CountElementsTest, Ranges_dont_overlap)
{
    const Ranges ranges{
        Range{2, 5},
        Range{9, 13},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 4 + 5 + 6;
    EXPECT_EQ(result, expected);
}

TEST(CountElementsTest, First_range_ends_within_second_range)
{
    const Ranges ranges{
        Range{2, 9},
        Range{5, 13},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 12 + 6;
    EXPECT_EQ(result, expected);
}

TEST(CountElementsTest, First_range_ends_after_second_range_but_before_third_range)
{
    const Ranges ranges{
        Range{2, 13},
        Range{5, 9},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 12 + 6;
    EXPECT_EQ(result, expected);
}

TEST(CountElementsTest, First_range_ends_within_third_range)
{
    const Ranges ranges{
        Range{2, 20},
        Range{5, 9},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 21;
    EXPECT_EQ(result, expected);
}

TEST(CountElementsTest, First_range_ends_after_third_range)
{
    const Ranges ranges{
        Range{2, 27},
        Range{5, 9},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 26;
    EXPECT_EQ(result, expected);
}

TEST(CountElementsTest, Second_range_starts_before_third_range_and_ends_within_third_range)
{
    const Ranges ranges{
        Range{2, 5},
        Range{9, 20},
        Range{17, 22}
    };
    const auto result = count_elements_in_ranges(ranges);
    constexpr auto expected = 4 + 14;
    EXPECT_EQ(result, expected);
}
