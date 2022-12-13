#pragma once

#include "utility/Dbg.hpp"
#include <regex>
#include <string>
#include <string_view>

namespace task
{
using Range = std::pair<int, int>;

bool is_fully_contained(const Range& r1, const Range& r2)
{
    return r1.first <= r2.first and r1.second >= r2.second;
}

bool are_overlapping(const Range& r1, const Range& r2)
{
    return (r1.first < r2.first) ? r1.second >= r2.first : r2.second >= r1.first;
}

auto parse_ranges(const std::string& string)
{
    std::smatch result;
    const std::regex regex{R"((\d+)-(\d+),(\d+)-(\d+))"};
    std::regex_search(string, result, regex);
    Range first_range{std::stoi(result.str(1)), std::stoi(result.str(2))};
    Range second_range{std::stoi(result.str(3)), std::stoi(result.str(4))};
    return std::pair<Range, Range>{first_range, second_range};
}

bool has_redundant_range(const std::string& string)
{
    const auto [first_range, second_range] = parse_ranges(string);
    return is_fully_contained(first_range, second_range) or is_fully_contained(second_range, first_range);
}

bool has_overlap(const std::string& string)
{
    DBG(string);
    const auto [first_range, second_range] = parse_ranges(string);
    return are_overlapping(first_range, second_range);
}
} // namespace task
