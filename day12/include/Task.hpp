#pragma once

#include "utility/Stream.hpp"
#include <algorithm>
#include <deque>
#include <memory>
#include <optional>
#include <ranges>
#include <unordered_set>
#include <vector>

namespace task
{
struct Vector { int x{0}, y{0}; };
bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }
Vector operator-(const Vector& lhs, const Vector& rhs) { return Vector{lhs.x - rhs.x, lhs.y - rhs.y}; }
Vector operator+(const Vector& lhs, const Vector& rhs) { return Vector{lhs.x + rhs.x, lhs.y + rhs.y}; }

struct VectorHash
{
    std::size_t operator()(const Vector& v) const { return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1); }
};

using Height = char;
using HeightMap = std::unordered_map<Vector, Height, VectorHash>;

struct ParseData
{
    HeightMap map;
    Vector source, target;
};
ParseData parse_input(utility::Stream& stream)
{
    ParseData data;
    auto y = 0;
    for (const auto& line : stream)
    {
        auto x = 0;
        for (auto c : line)
        {
            Vector current{x, y};
            if (c == 'S')
            {
                data.source = current;
                c = 'a';
            }
            else if (c == 'E')
            {
                data.target = current;
                c = 'z';
            }
            data.map.emplace(current, c);
            ++x;
        }
        ++y;
    }
    return data;
}

using VectorList = std::vector<Vector>;

template <typename ClimbStrategy>
class BreadthFirstSearcher
{
public:
    BreadthFirstSearcher(const HeightMap& height_map, Vector start) :
        height_map{height_map}
    {
        Node start_node{start, nullptr};
        visited_nodes.emplace(start, start_node);
        queue_to_check.push_back(start_node);
        current_node = start_node;
    }

    std::optional<Vector> explore_next()
    {
        if (queue_to_check.empty())
        {
            return std::nullopt;
        }
        current_node = queue_to_check.front();
        queue_to_check.pop_front();

        const auto neighbors = get_neighbors(current_node.position);
        for (const auto& neighbor : neighbors)
        {
            if (const auto visited_itr = visited_nodes.find(neighbor); visited_itr == visited_nodes.end())
            {
                const Node new_node{neighbor, std::make_shared<Node>(current_node)};
                visited_nodes.emplace(neighbor, new_node);
                queue_to_check.push_back(new_node);
            }
        }
        return current_node.position;
    }

    VectorList get_path() const
    {
        VectorList out{current_node.position};
        auto current = current_node.parent;
        while (current)
        {
            out.push_back(current->position);
            current = current->parent;
        }
        return out;
    }

private:
    struct Node
    {
        Vector position;
        std::shared_ptr<Node> parent{nullptr};
    };
    using NodeMap = std::unordered_map<Vector, Node, VectorHash>;

    const HeightMap& height_map;
    NodeMap visited_nodes;
    ClimbStrategy climb_strategy;
    std::deque<Node> queue_to_check;
    Node current_node;

    bool is_available(const Vector& source, const Vector& target)
    {
        const auto source_node = height_map.find(source);
        const auto target_node = height_map.find(target);
        if (source_node == height_map.end() or target_node == height_map.end())
        {
            return false;
        }
        return climb_strategy(source_node->second, target_node->second);
    }

    VectorList generate_sides(const Vector& source)
    {
        return {
            Vector{source.x, source.y + 1},
            Vector{source.x, source.y - 1},
            Vector{source.x - 1, source.y},
            Vector{source.x + 1, source.y},
        };
    }

    VectorList get_neighbors(const Vector& source)
    {
        auto neighbors = generate_sides(source);
        VectorList possible_neighbors;
        const auto is_possible_to_move = [this, &source](const Vector& target) { return is_available(source, target); };
        std::ranges::move(neighbors | std::views::filter(is_possible_to_move), std::back_inserter(possible_neighbors));
        return possible_neighbors;
    }
};

struct Ascend
{
    bool operator()(char source, char target)
    {
        return static_cast<int>(target) - static_cast<int>(source) <= 1;
    }
};

auto find_shortest_path(utility::Stream& stream)
{
    auto [map, source, target] = parse_input(stream);
    BreadthFirstSearcher<Ascend> searcher{map, source};
    for (auto current_step = searcher.explore_next(); current_step.has_value(); current_step = searcher.explore_next())
    {
        if (current_step.value() == target)
        {
            const auto path = searcher.get_path();
            const auto steps_taken = path.size() - 1;
            return steps_taken;
        }
    }
    throw std::logic_error{"Could not find target"};
}

struct Descend
{
    bool operator()(char source, char target)
    {
        return static_cast<int>(target) - static_cast<int>(source) >= -1;
    }
};

auto find_shortest_path_from_any_lowest(utility::Stream& stream)
{
    auto [map, _, source] = parse_input(stream);
    BreadthFirstSearcher<Descend> searcher{map, source};
    std::optional<std::size_t> shortest_found;
    for (auto current_step = searcher.explore_next(); current_step.has_value(); current_step = searcher.explore_next())
    {
        if (map.at(current_step.value()) == 'a')
        {
            const auto path = searcher.get_path();
            const auto steps_taken = path.size() - 1;
            if (shortest_found.has_value())
            {
                shortest_found = std::min(shortest_found.value(), steps_taken);
            }
            else
            {
                shortest_found = steps_taken;
            }
        }
    }
    if (not shortest_found.has_value())
    {
        throw std::logic_error{"Could not find target"};
    }
    return shortest_found.value();
}
} // namespace task
