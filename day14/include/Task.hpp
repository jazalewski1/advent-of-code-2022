#pragma once

#include "utility/Stream.hpp"
#include <algorithm>
#include <optional>
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
        const auto first = std::min(begin.x, end.x);
        const auto last = std::max(begin.x, end.x);
        for (auto x = first; x <= last; ++x)
        {
            target.emplace(x, begin.y);
        }
    }
    else
    {
        const auto first = std::min(begin.y, end.y);
        const auto last = std::max(begin.y, end.y);
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

std::optional<Vector> find_next_position(const Vector& source, const VectorSet& elements)
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

auto count_stable_units_to_abyss(utility::Stream& stream)
{
    auto stable_elements = parse_input(stream);
    const auto lowest_height = find_lowest_height(stable_elements);
    const Vector start{500, 0};

    auto element_counter = 0;
    std::vector<Vector> unstable_stack{start};
    while (not unstable_stack.empty())
    {
        const auto current = unstable_stack.back();
        const auto next = find_next_position(current, stable_elements);
        if (next.has_value())
        {
            unstable_stack.push_back(next.value());
        }
        else
        {
            unstable_stack.pop_back();
            stable_elements.insert(current);
            ++element_counter;
        }
        if (current.y == lowest_height)
        {
            break;
        }
    }

    return element_counter;
}

auto count_stable_units_to_start(utility::Stream& stream)
{
    auto stable_elements = parse_input(stream);
    const auto lowest_height = find_lowest_height(stable_elements);
    const auto ground_height = lowest_height + 2;
    const Vector start{500, 0};

    auto element_counter = 0;
    std::vector<Vector> unstable_stack{start};
    while (not unstable_stack.empty())
    {
        const auto current = unstable_stack.back();
        const auto next = find_next_position(current, stable_elements);
        if (next.has_value() and current.y + 1 != ground_height)
        {
            unstable_stack.push_back(next.value());
        }
        else
        {
            unstable_stack.pop_back();
            stable_elements.insert(current);
            ++element_counter;
        }
    }
    return element_counter;
}
} // namespace task
