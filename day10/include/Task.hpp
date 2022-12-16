#pragma once

#include "utility/Stream.hpp"
#include <numeric>
#include <optional>
#include <vector>

namespace task
{
std::optional<int> parse_command(const std::string& string)
{
    constexpr auto noop_string = "noop";
    if (string == noop_string)
    {
        return std::nullopt;
    }
    return std::stoi(string.substr(5));
}

using Values = std::vector<int>;

auto process_commands(utility::Stream& stream)
{
    auto value_x = 1;
    Values values{value_x};
    for (const auto& line : stream)
    {
        const auto command = parse_command(line);
        if (command.has_value())
        {
            values.push_back(value_x);
            values.push_back(value_x);
            value_x += command.value();
        }
        else
        {
            values.push_back(value_x);
        }
    }
    return values;
}

using WithIndices = std::initializer_list<int>;
auto accumulate_strengths(const Values& values, WithIndices&& indices)
{
    return std::accumulate(indices.begin(), indices.end(), 0, [&](auto s, auto i) { return s + values.at(i) * i; });
}

auto accumulate_strengths(utility::Stream& stream)
{
    const auto values_per_cycle = process_commands(stream);
    return accumulate_strengths(values_per_cycle, WithIndices{20, 60, 100, 140, 180, 220});
}

auto sprite_contains(int sprite_middle, int pixel)
{
    return sprite_middle - 1 == pixel or sprite_middle == pixel or sprite_middle + 1 == pixel;
}

auto render_crt(utility::Stream& stream)
{
    const auto values_per_cycle = process_commands(stream);
    for (auto cycle = 1u; cycle < values_per_cycle.size(); ++cycle)
    {
        const auto sprite_middle = values_per_cycle.at(cycle);
        const auto pixel_position = (cycle - 1) % 40;
        std::cout << (sprite_contains(sprite_middle, pixel_position) ? '#' : '.');
        if (cycle % 40 == 0)
        {
            std::cout << '\n';
        }
    }
}
} // namespace task
