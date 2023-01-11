#pragma once

#include "utility/Stream.hpp"
#include <regex>
#include <unordered_set>
#include <vector>

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

auto find_empty_xs(const Sensors& sensors, int row)
{
    std::unordered_set<int> empty_xs;
    for (const auto& sensor : sensors)
    {
        const auto y_distance_to_row = std::abs(sensor.position.y - row);
        const auto radius = distance(sensor.position, sensor.beacon_position);
        if (y_distance_to_row > radius)
        {
            continue;
        }
        const auto half_x_distance_at_row = radius - y_distance_to_row;
        const auto x_start = sensor.position.x - half_x_distance_at_row;
        const auto x_end = sensor.position.x + half_x_distance_at_row;
        for (auto x = x_start; x <= x_end; ++x)
        {
            if (x == sensor.beacon_position.x and row == sensor.beacon_position.y)
            {
                continue;
            }
            empty_xs.insert(x);
        }
    }
    return empty_xs;
}

auto count_positions(utility::Stream& stream, int scanned_row)
{
    auto sensors = parse_input(stream);
    const auto empty_xs = find_empty_xs(sensors, scanned_row);
    return empty_xs.size();
}
} // namespace task
