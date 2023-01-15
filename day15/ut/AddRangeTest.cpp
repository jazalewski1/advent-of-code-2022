#include "Task.hpp"
#include <gtest/gtest.h>

using namespace task;

TEST(AddRangeTest, Add_first_range)
{
    Ranges ranges{};
    Range new_range{3, 8};
    add_range(ranges, new_range);
    const Ranges expected{
        {3, 8}
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_and_ends_before_first_element)
{
    Ranges ranges{
        {6, 9}
    };
    Range new_range{2, 4};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 4},
        {6, 9},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_before_first_element_and_ends_within_first_element)
{
    Ranges ranges{
        {4, 9}
    };
    Range new_range{2, 6};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 9},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_before_first_element_and_ends_after_first_element)
{
	Ranges ranges{{4, 9}, {14, 17}};
	Range new_range{2, 11};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 11},
        {14, 17}
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_before_first_element_and_ends_within_next_element)
{
	Ranges ranges{{4, 9}, {14, 19}};
	Range new_range{2, 16};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 19},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_within_first_element_and_ends_within_first_element)
{
	Ranges ranges{{4, 13}};
	Range new_range{7, 11};
    add_range(ranges, new_range);
    const Ranges expected{
        {4, 13},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_within_first_element_and_ends_before_next_element)
{
	Ranges ranges{{4, 9}, {13, 17}};
	Range new_range{7, 11};
    add_range(ranges, new_range);
    const Ranges expected{
        {4, 11},
        {13, 17},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_within_first_element_and_ends_within_next_element)
{
	Ranges ranges{{3, 9}, {11, 17}};
	Range new_range{7, 14};
    add_range(ranges, new_range);
    const Ranges expected{
        {3, 17},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_after_first_element_and_ends_before_next_element)
{
	Ranges ranges{{2, 6}, {13, 17}};
	Range new_range{9, 11};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 6},
        {9, 11},
        {13, 17},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_after_first_element_and_ends_within_next_element)
{
	Ranges ranges{{2, 5}, {11, 17}};
	Range new_range{7, 13};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {7, 17},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_and_ends_within_next_element)
{
	Ranges ranges{{2, 5}, {11, 17}};
	Range new_range{13, 15};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {11, 17},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_within_next_element_and_ends_after_next_element)
{
	Ranges ranges{{2, 5}, {11, 15}};
	Range new_range{13, 18};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {11, 18},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_and_ends_after_next_element)
{
	Ranges ranges{{2, 5}, {11, 15}};
	Range new_range{17, 21};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {11, 15},
        {17, 21},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_before_first_element_and_ends_after_next_element)
{
	Ranges ranges{{4, 9}, {14, 19}};
	Range new_range{2, 21};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 21},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_overlaps_with_multiple_elements)
{
	Ranges ranges{{2, 9}, {14, 19}, {23, 30}, {35, 37}, {41, 48}, {56, 60}};
	Range new_range{25, 45};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 9},
        {14, 19},
        {23, 48},
        {56, 60},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_touching_first_element_and_ends_before_next_element)
{
	Ranges ranges{{2, 5}, {12, 16}};
	Range new_range{6, 10};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {6, 10},
        {12, 16},
    };
    EXPECT_EQ(ranges, expected);
}

TEST(AddRangeTest, New_range_starts_after_first_element_and_ends_touching_next_element)
{
	Ranges ranges{{2, 5}, {12, 16}};
	Range new_range{8, 11};
    add_range(ranges, new_range);
    const Ranges expected{
        {2, 5},
        {8, 11},
        {12, 16},
    };
    EXPECT_EQ(ranges, expected);
}
