#include "Task.hpp"
#include <gtest/gtest.h>

using namespace task;

TEST(GetRangeTest, Sensor_area_does_not_reach_searched_row)
{
    const Vector sensor_position{7, 3};
    const Vector beacon_position{10, 5};
    const Sensor sensor{sensor_position, beacon_position};
    constexpr auto searched_row = 10;
    const auto result = get_range(sensor, searched_row);
    EXPECT_FALSE(result.has_value());
}

TEST(GetRangeTest, Sensor_area_touches_searched_row_with_last_row)
{
    const Vector sensor_position{5, 6};
    const Vector beacon_position{8, 5};
    const Sensor sensor{sensor_position, beacon_position};
    constexpr auto searched_row = 10;
    const auto result = get_range(sensor, searched_row);
    ASSERT_TRUE(result.has_value());
    const Range expected{5, 6};
    EXPECT_EQ(result.value(), expected);
}

TEST(GetRangeTest, Sensor_area_covers_range_in_searched_row)
{
    const Vector sensor_position{9, 7};
    const Vector beacon_position{5, 8};
    const Sensor sensor{sensor_position, beacon_position};
    constexpr auto searched_row = 10;
    const auto result = get_range(sensor, searched_row);
    ASSERT_TRUE(result.has_value());
    const Range expected{7, 12};
    EXPECT_EQ(result.value(), expected);
}

TEST(GetRangeTest, Sensor_area_covers_range_in_searched_row_with_beacon_on_the_right)
{
    const Vector sensor_position{10, 8};
    const Vector beacon_position{13, 10};
    const Sensor sensor{sensor_position, beacon_position};
    constexpr auto searched_row = 10;
    const auto result = get_range(sensor, searched_row);
    ASSERT_TRUE(result.has_value());
    const Range expected{7, 13};
    EXPECT_EQ(result.value(), expected);
}

TEST(GetRangeTest, Sensor_area_touches_searched_row_but_there_is_beacon)
{
    const Vector sensor_position{3, 6};
    const Vector beacon_position{3, 10};
    const Sensor sensor{sensor_position, beacon_position};
    constexpr auto searched_row = 10;
    const auto result = get_range(sensor, searched_row);
    ASSERT_FALSE(result.has_value());
}
