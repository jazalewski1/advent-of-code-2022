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
bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }

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

auto count_elements(const Range& range)
{
    return range.second - range.first;
}

std::optional<Range> get_range(const Sensor& sensor, int row)
{
    const auto& beacon = sensor.beacon_position;
    const auto y_distance_to_row = std::abs(sensor.position.y - row);
    const auto radius = distance(sensor.position, beacon);
    if (y_distance_to_row > radius)
    {
        return std::nullopt;
    }
    const auto x_half_distance_at_row = radius - y_distance_to_row;
    auto first = sensor.position.x - x_half_distance_at_row;
    auto last = sensor.position.x + x_half_distance_at_row;
    if (last - first == 1 and beacon == Vector{first, row})
    {
        return std::nullopt;
    }
    if (first == beacon.x)
    {
        first += 1;
    }
    if (last == beacon.x)
    {
        last -= 1;
    }
    return Range{first, last + 1};
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

auto find_empty_ranges(const Sensors& sensors, int row)
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

auto count_elements(const Ranges& ranges)
{
    auto sum = 0;
    for (const auto& range : ranges)
    {
        sum += count_elements(range);
    }
    return sum;
}

auto count_empty_positions(utility::Stream& stream, int scanned_row)
{
    auto sensors = parse_input(stream);
    const auto empty_ranges = find_empty_ranges(sensors, scanned_row);
    return count_elements(empty_ranges);
}

auto find_unknown_ranges(const Sensors& sensors, int row, const Range& limit_range)
{
    Ranges unknown_ranges;
    const auto empty_ranges = find_empty_ranges(sensors, row);
    const auto overlaps_with_limit = [&limit_range](const Range& r) { return are_overlapping(limit_range, r); };
    const auto first_overlapping = std::find_if(empty_ranges.begin(), empty_ranges.end(), overlaps_with_limit);
    const auto first_not_overlapping = std::find_if_not(first_overlapping, empty_ranges.end(), overlaps_with_limit);
    if (first_overlapping == empty_ranges.end())
    {
        return unknown_ranges;
    }

    if (limit_range.first < first_overlapping->first)
    {
        unknown_ranges.emplace(limit_range.first, first_overlapping->first);
    }

    auto first_of_current_unknown = first_overlapping->second;
    for (auto current = std::next(first_overlapping); current != first_not_overlapping; ++current)
    {
        const auto second_of_current_unknown = current->first;
        if (second_of_current_unknown - first_of_current_unknown >= 1)
        {
            unknown_ranges.emplace(first_of_current_unknown, second_of_current_unknown);
        }
        first_of_current_unknown = current->second;
    }
    auto last_overlapping = std::prev(first_not_overlapping);
    if (last_overlapping->second < limit_range.second)
    {
        unknown_ranges.emplace(last_overlapping->second, limit_range.second);
    }

    return unknown_ranges;
}

auto get_positions(const Ranges& ranges, int y)
{
    std::vector<Vector> positions;
    for (const auto& range : ranges)
    {
        for (auto x = range.first; x < range.second; ++x)
        {
            positions.emplace_back(x, y);
        }
    }
    return positions;
}

auto is_beacon(const Sensors& sensors, const Vector& position)
{
    return std::ranges::any_of(sensors, [&position](const Sensor& sensor) {
        return sensor.beacon_position.x == position.x and sensor.beacon_position.y == position.y;
    });
}

void remove_beacons(std::vector<Vector>& positions, const Sensors& sensors)
{
    const auto is_beacon_position = [&sensors](const Vector& position) { return is_beacon(sensors, position); };
    const auto removal_itr = std::remove_if(positions.begin(), positions.end(), is_beacon_position);
    positions.erase(removal_itr, positions.end());
}

auto calculate_frequency(utility::Stream& stream)
{
    const auto sensors = parse_input(stream);
    constexpr auto min = 0;
    constexpr auto max = 4'000'000;
    constexpr Range limit_range{min, max};
    for (auto y = min; y <= max; ++y)
    {
        const auto unknown_ranges = find_unknown_ranges(sensors, y, limit_range);
        const auto num_of_unknown_positions = count_elements(unknown_ranges);
        if (num_of_unknown_positions > 1)
        {
            continue;
        }
        auto unknown_positions = get_positions(unknown_ranges, y);
        remove_beacons(unknown_positions, sensors);
        if (not unknown_positions.empty())
        {
            const auto beacon = unknown_positions.front();
            const auto frequency = (static_cast<unsigned long long int>(beacon.x) * max) + beacon.y;
            return frequency;
        }
    }
    return 0ull;
}
} // namespace task
