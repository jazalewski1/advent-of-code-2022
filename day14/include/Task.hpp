#pragma once

#include "utility/Stream.hpp"
#include <ranges>
#include <regex>
#include <unordered_set>
#include <vector>

namespace task
{
struct Vector { int x{0}, y{0}; };
bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }
struct VectorHash
{
    std::size_t operator()(const Vector& v) const { return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1); }
};

using VectorSet = std::unordered_set<Vector, VectorHash>;

void add_positions_from_path(VectorSet& target, const Vector& begin, const Vector& end)
{
    if (begin.x != end.x)
    {
        const auto first = begin.x < end.x ? begin.x : end.x;
        const auto last = begin.x < end.x ? end.x : begin.x;
        for (auto x = first; x <= last; ++x)
        {
            target.emplace(x, begin.y);
        }
    }
    else
    {
        const auto first = begin.y < end.y ? begin.y : end.y;
        const auto last = begin.y < end.y ? end.y : begin.y;
        for (auto y = first; y <= last; ++y)
        {
            target.emplace(begin.x, y);
        }
    }
}

VectorSet parse_input(utility::Stream& stream)
{
    const std::regex regex{R"((\d+),(\d+))"};
    const auto parse_vector = [](const auto& m) { return Vector{std::stoi(m.str(1)), std::stoi(m.str(2))}; };
    VectorSet out;
    for (const auto& line : stream)
    {
        const auto match_begin = std::sregex_iterator(line.begin(), line.end(), regex);
        const auto match_end = std::sregex_iterator{};
        auto previous_vector = parse_vector(*match_begin);
        for (auto current_match = std::next(match_begin); current_match != match_end; ++current_match)
        {
            const auto current_vector = parse_vector(*current_match);
            add_positions_from_path(out, previous_vector, current_vector);
            previous_vector = current_vector;
        }
    }
    return out;
}

auto find_lowest_height(const VectorSet& vectors)
{
    return std::ranges::max_element(vectors, [](const auto& lhs, const auto& rhs) { return lhs.y < rhs.y; })->y;
}

std::optional<Vector> get_next_position(const Vector& source, const VectorSet& elements)
{
    if (const Vector below{source.x, source.y + 1}; not elements.contains(below))
    {
        return below;
    }
    if (const Vector left_diag{source.x - 1, source.y + 1}; not elements.contains(left_diag))
    {
        return left_diag;
    }
    if (const Vector right_diag{source.x + 1, source.y + 1}; not elements.contains(right_diag))
    {
        return right_diag;
    }
    return std::nullopt;
}

Vector simulate_element(const Vector& start, const VectorSet& stable_elements, int stopping_height)
{
    auto current = start;
    for (auto next = get_next_position(current, stable_elements); next.has_value(); next = get_next_position(current, stable_elements))
    {
        if (next.value().y == stopping_height)
        {
            break;
        }
        current = next.value();
    }
    return current;
}

auto count_stable_units_to_abyss(utility::Stream& stream)
{
    auto stable_elements = parse_input(stream);
    const auto lowest_height = find_lowest_height(stable_elements);
    const auto below_lowest_height = lowest_height + 1;
    const Vector start{500, 0};

    auto element_counter = 0;
    auto new_element = simulate_element(start, stable_elements, below_lowest_height);
    while (new_element.y != lowest_height)
    {
        stable_elements.insert(new_element);
        ++element_counter;
        new_element = simulate_element(start, stable_elements, below_lowest_height);
    }
    return element_counter;
}

auto count_stable_units_to_start(utility::Stream& stream)
{
    auto stable_elements = parse_input(stream);
    const auto lowest_height = find_lowest_height(stable_elements);
    const auto ground_height = lowest_height + 2;
    const Vector start{500, 0};

    auto new_element = simulate_element(start, stable_elements, ground_height);
    stable_elements.insert(new_element);
    auto element_counter = 1;
    while (new_element != start)
    {
        new_element = simulate_element(start, stable_elements, ground_height);
        stable_elements.insert(new_element);
        ++element_counter;
    }
    return element_counter;
}
} // namespace task
