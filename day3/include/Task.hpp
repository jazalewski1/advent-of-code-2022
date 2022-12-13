#pragma once

#include "utility/Dbg.hpp"
#include <algorithm>
#include <string>
#include <ranges>

namespace task
{
struct IsIn
{
    std::string string;
    bool operator()(auto c) { return std::ranges::find(string, c) != string.end(); }
};

auto get_common_letter(const std::string& string)
{
    const auto half = string.size() / 2;
    const auto half_itr = std::next(string.begin(), half);
    const auto is_in_first_half = [&](auto c) { return std::find(string.begin(), half_itr, c) != half_itr; };
    if (const auto common_itr = std::find_if(half_itr, string.end(), is_in_first_half); common_itr != string.end())
    {
        return *common_itr;
    }
    throw std::logic_error{"Wrong input - no common letter found"};
}

auto get_common_letters(const std::string& s1, const std::string& s2)
{
    std::string result;
    std::ranges::copy_if(s2, std::back_inserter(result), IsIn{s1});
    return result;
}

auto get_common_letter(const std::string& s1, const std::string& s2)
{
    if (const auto found_common = std::ranges::find_if(s1, IsIn{s2}); found_common != s1.end())
    {
        return *found_common;
    }
    throw std::logic_error{"Wrong input - no common letter found"};
}

template <typename ... Strings>
auto get_common_letter(const std::string& s1, const std::string& s2, const Strings& ... strings)
{
    return get_common_letter(get_common_letters(s1, s2), strings...);
}

auto convert_to_priority(char c)
{
    const auto priority_shift = std::isupper(c) ? 38 : 96;
    return static_cast<int>(c) - priority_shift;
}
} // namespace task
