#pragma once

#include "utility/Dbg.hpp"
#include <algorithm>
#include <string>
#include <ranges>

namespace task
{
auto are_all_elements_unique(std::string::const_iterator begin, std::string::const_iterator end)
{
    for (auto itr = begin; itr != end; ++itr)
    {
        if (std::find(itr + 1, end, *itr) != end)
        {
            return false;
        }
    }
    return true;
}

unsigned count_characters_to_marker_end(const std::string& string, unsigned length)
{
    // TODO: could use std::views::slide :(
    for (auto marker_end = std::next(string.begin(), length); std::prev(marker_end) != string.end(); ++marker_end)
    {
        const auto marker_begin = std::prev(marker_end, length);
        if (are_all_elements_unique(marker_begin, marker_end))
        {
            return std::distance(string.begin(), marker_end);
        }
    }
    return 0u;
}
} // namespace task
