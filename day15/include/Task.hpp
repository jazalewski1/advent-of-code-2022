#pragma once

#include "utility/Stream.hpp"
#include <optional>
#include <regex>
#include <set>
#include <vector>
#include <ranges>

namespace task
{
struct Vector { int x{0}, y{0}; };

struct Sensor
{
    Vector position;
    Vector beacon_position;
};

using Sensors = std::vector<Sensor>;
Sensors parse_input(utility::Stream& stream)
{
    const std::regex regex{R"(^.*?x=(-?\d+), y=(-?\d+).*?x=(-?\d+), y=(-?\d+))"};
    Sensors out;
    for (const auto& line : stream)
    {
        std::smatch match;
        std::regex_search(line, match, regex);
        const Vector position{std::stoi(match.str(1)), std::stoi(match.str(2))};
        const Vector beacon_position{std::stoi(match.str(3)), std::stoi(match.str(4))};
        out.emplace_back(position, beacon_position);
    }
    return out;
}

auto distance(const Vector& lhs, const Vector& rhs)
{
    const auto dist_x = std::abs(lhs.x - rhs.x);
    const auto dist_y = std::abs(lhs.y - rhs.y);
    return dist_x + dist_y;
}

using Range = std::pair<int, int>;
struct RangeComparator
{
    bool operator()(const Range& lhs, const Range& rhs) const { return lhs.first < rhs.first; }
};
using Ranges = std::set<Range, RangeComparator>;

std::optional<Range> get_range(const Sensor& sensor, int row)
{
    const auto y_distance_to_row = std::abs(sensor.position.y - row);
    const auto radius = distance(sensor.position, sensor.beacon_position);
    if (y_distance_to_row > radius)
    {
        return std::nullopt;
    }
    const auto x_half_distance_at_row = radius - y_distance_to_row;
    auto x_start = sensor.position.x - x_half_distance_at_row;
    auto x_end = sensor.position.x + x_half_distance_at_row;
    if (x_start == x_end and sensor.beacon_position.x == x_start and sensor.beacon_position.y == row)
    {
        return std::nullopt;
    }
    if (x_start == sensor.beacon_position.x)
    {
        x_start += 1;
    }
    if (x_end == sensor.beacon_position.x)
    {
        x_end -= 1;
    }
    return Range{x_start, x_end};
}

bool are_overlapping(const Range& r1, const Range& r2)
{
    return (r1.first < r2.first) ? r1.second >= r2.first : r2.second >= r1.first;
}

void add_range(Ranges& ranges, const Range& new_range)
{
    const auto overlaps_with_new = [&new_range](const Range& r) { return are_overlapping(new_range, r); };
    const auto first_overlapping = std::find_if(ranges.begin(), ranges.end(), overlaps_with_new);
    if (first_overlapping != ranges.end())
    {
        const auto first_not_overlapping = std::find_if_not(first_overlapping, ranges.end(), overlaps_with_new);
        const auto last_overlapping = std::prev(first_not_overlapping);
        const auto first_to_insert = std::min(new_range.first, first_overlapping->first);
        const auto second_to_insert = std::max(new_range.second, last_overlapping->second);
        ranges.erase(first_overlapping, first_not_overlapping);
        ranges.emplace(first_to_insert, second_to_insert);
    }
    else
    {
        ranges.insert(new_range);
    }
}

auto find_empty_x_ranges(const Sensors& sensors, int row)
{
    Ranges empty_ranges;
    for (const auto& sensor : sensors)
    {
        const auto range = get_range(sensor, row);
        if (range.has_value())
        {
            add_range(empty_ranges, *range);
        }
    }
    return empty_ranges;
}

auto count_elements_in_range(const Range& range)
{
    return (range.second - range.first) + 1;
}

auto count_elements_in_ranges(const Ranges& ranges)
{
    auto sum = 0;
    for (const auto& range : ranges)
    {
        sum += count_elements_in_range(range);
    }
    return sum;
}

auto count_empty_positions(utility::Stream& stream, int scanned_row)
{
    auto sensors = parse_input(stream);
    const auto empty_ranges = find_empty_x_ranges(sensors, scanned_row);
    return count_elements_in_ranges(empty_ranges);
}
} // namespace task
