#pragma once

#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

namespace task
{
struct Vector { int x, y; };
std::ostream& operator<<(std::ostream& s, const Vector& v) { return s << '{' << v.x << ", " << v.y << '}'; }

class Map2D
{
public:
    Map2D(utility::Stream& stream)
    {
        std::ranges::copy(stream, std::back_inserter(strings));
    }

    Vector size() const { return {static_cast<int>(strings.at(0).length()), static_cast<int>(strings.size())}; }

    char at(const Vector& v) const { return strings.at(v.y).at(v.x); }

    auto get_left_range(const Vector& target) const
    {
        std::string out;
        const auto& row = strings.at(target.y);
        std::copy(row.rend() - target.x, row.rend(), std::back_inserter(out));
        return out;
    }

    auto get_right_range(const Vector& target) const
    {
        std::string out;
        const auto& row = strings.at(target.y);
        std::copy(row.begin() + target.x + 1, row.end(), std::back_inserter(out));
        return out;
    }

    auto get_top_range(const Vector& target) const
    {
        std::string out;
        std::transform(strings.rend() - target.y,
                       strings.rend(),
                       std::back_inserter(out),
                       [x = target.x](const auto& row) { return row.at(x); });
        return out;
    }

    auto get_bottom_range(const Vector& target) const
    {
        std::string out;
        std::transform(strings.begin() + target.y + 1,
                       strings.end(),
                       std::back_inserter(out),
                       [x = target.x](const auto& row) { return row.at(x); });
        return out;
    }

private:
    std::vector<std::string> strings;
};

auto is_visible(const std::string& string, char target_value)
{
    const auto lower = [target_value](auto c) { return c < target_value; };
    return std::ranges::all_of(string, lower);
}

auto is_visible_from_all_sides(const Map2D& map, Vector target)
{
    const auto target_value = map.at(target);
    const auto left = map.get_left_range(target);
    const auto left_visible = is_visible(left, target_value);
    const auto right = map.get_right_range(target);
    const auto right_visible = is_visible(right, target_value);
    const auto top = map.get_top_range(target);
    const auto top_visible = is_visible(top, target_value);
    const auto bottom = map.get_bottom_range(target);
    const auto bottom_visible = is_visible(bottom, target_value);
    return left_visible or right_visible or top_visible or bottom_visible;
}

auto count_inner_trees_visibility(const Map2D& map)
{
    const auto size = map.size();
    auto result = 0;
    for (auto y : std::views::iota(int{1}, size.y - 1))
    {
        for (auto x : std::views::iota(int{1}, size.x - 1))
        {
            const Vector current{x, y};
            result += is_visible_from_all_sides(map, current) ? 1 : 0;
        }
    }
    return result;
}

auto count_sides_visibility(const Map2D& map)
{
    const auto size = map.size();
    return (size.x * 2) + ((size.y - 2) * 2);
}

auto count_visible_trees(utility::Stream& stream)
{
    Map2D map{stream};
    return count_inner_trees_visibility(map) + count_sides_visibility(map);
}

auto count_score(const std::string& string, char target_value)
{
    const auto greater = [target_value](auto c) { return c >= target_value; };
    const auto found = std::ranges::find_if(string, greater);
    const auto result = std::distance(string.begin(), found);
    return found == string.end() ? result : result + 1;
}

auto count_scenic_score_for_all_sides(const Map2D& map, Vector target)
{
    const auto target_value = map.at(target);
    const auto left = map.get_left_range(target);
    const auto left_score = count_score(left, target_value);
    const auto right = map.get_right_range(target);
    const auto right_score = count_score(right, target_value);
    const auto top = map.get_top_range(target);
    const auto top_score = count_score(top, target_value);
    const auto bottom = map.get_bottom_range(target);
    const auto bottom_score = count_score(bottom, target_value);
    return left_score * right_score * top_score * bottom_score;
}

auto find_max_scenic_score(const Map2D& map)
{
    const auto size = map.size();
    auto max = 0l;
    for (auto y : std::views::iota(int{1}, size.y - 1))
    {
        for (auto x : std::views::iota(int{1}, size.x - 1))
        {
            const Vector current{x, y};
            max = std::max(max, count_scenic_score_for_all_sides(map, current));
        }
    }
    return max;
}

auto get_max_scenic_score(utility::Stream& stream)
{
    Map2D map{stream};
    return find_max_scenic_score(map);
}
} // namespace task
