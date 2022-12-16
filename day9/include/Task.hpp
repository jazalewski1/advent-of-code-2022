#pragma once

#include "utility/Stream.hpp"
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

namespace task
{
struct Vector { int x, y; };
bool operator==(const Vector& lhs, const Vector& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }
Vector operator-(const Vector& lhs, const Vector& rhs) { return Vector{lhs.x - rhs.x, lhs.y - rhs.y}; }
Vector operator+(const Vector& lhs, const Vector& rhs) { return Vector{lhs.x + rhs.x, lhs.y + rhs.y}; }

struct VectorHash
{
    std::size_t operator()(const Vector& v) const { return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1); }
};

std::tuple<char, int> parse_move(const std::string& input)
{
    const auto direction = input.front();
    const auto count = std::stoi(input.substr(2, input.size()));
    return {direction, count};
}

Vector get_vector(char direction)
{
    Vector out{0, 0};
    switch (direction)
    {
        case 'U': ++out.y; break;
        case 'D': --out.y; break;
        case 'L': --out.x; break;
        case 'R': ++out.x; break;
    }
    return out;
}

bool are_touching(const Vector& v1, const Vector& v2)
{
    return std::abs(v1.x - v2.x) <= 1 and std::abs(v1.y - v2.y) <= 1;
}

Vector normalize(Vector v)
{
    v.x = v.x != 0 ? v.x / std::abs(v.x) : 0;
    v.y = v.y != 0 ? v.y / std::abs(v.y) : 0;
    return v;
}

class Rope
{
public:
    Rope(unsigned size) : elements(size, Vector{0, 0}) {}

    void move(char direction)
    {
        auto& head = elements.front();
        head = head + get_vector(direction);

        for (auto itr = elements.begin() + 1; itr != elements.end(); ++itr)
        {
            const auto& in_front = *std::prev(itr);
            auto& current = *itr;

            if (are_touching(current, in_front))
            {
                break;
            }

            const auto diff = in_front - current;
            current = current + normalize(diff);
        }
    }

    Vector get_tail() const { return elements.back(); }

private:
    std::vector<Vector> elements;
};

auto count_unique_positions(utility::Stream& stream, unsigned length)
{
    std::unordered_set<Vector, VectorHash> tail_history;
    Rope rope{length};
    for (const auto& line : stream)
    {
        const auto [direction, count] = parse_move(line);
        for ([[maybe_unused]] auto i : std::views::iota(0, count))
        {
            rope.move(direction);
            tail_history.insert(rope.get_tail());
        }
    }
    return tail_history.size();
}
} // namespace task
