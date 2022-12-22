#pragma once

#include "utility/Stream.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace task
{
using Value = int;
struct Element;
using List = std::vector<Element>;
struct Element { std::variant<Value, List> type; };

List parse_input(const std::string& string)
{
    List out;
    auto* current_list = &out;
    std::vector<List*> lists;
    for (auto current = string.cbegin(); current != string.cend(); ++current)
    {
        if (*current == '[')
        {
            lists.push_back(current_list);
            auto& new_element = current_list->emplace_back(List{});
            auto& new_list = std::get<List>(new_element.type);
            current_list = &new_list;
        }
        else if (*current == ']')
        {
            current_list = lists.back();
            lists.pop_back();
        }
        else if (*current != ',')
        {
            auto value_start = current;
            while (*current >= '0' and *current <= '9')
            {
                std::advance(current, 1);
            }
            const std::string_view value_view{value_start, current};
            current_list->emplace_back(std::stoi(value_view.data()));
        }
    }
    return out;
}

enum class Result {Correct, Incorrect, Unresolved};

Result is_correct_order(const List& lhs, const List& rhs);

Result is_correct_order(const Value& lhs, const Value& rhs)
{
    if (lhs == rhs)
    {
        return Result::Unresolved;
    }
    return lhs < rhs ? Result::Correct : Result::Incorrect;
}

Result is_correct_order(const Value& lhs, const List& rhs)
{
    const List lhs_list{Element{lhs}};
    return is_correct_order(lhs_list, rhs);
}

Result is_correct_order(const List& lhs, const Value& rhs)
{
    const List rhs_list{Element{rhs}};
    return is_correct_order(lhs, rhs_list);
}

Result is_correct_order(const Element& lhs, const Element& rhs)
{
    return std::visit([](const auto& lhs, const auto& rhs) { return is_correct_order(lhs, rhs); }, lhs.type, rhs.type);
}

Result is_correct_order(const List& lhs, const List& rhs)
{
    auto lhs_itr = lhs.begin();
    auto rhs_itr = rhs.begin();
    while (lhs_itr != lhs.end() and rhs_itr != rhs.end())
    {
        const auto result = is_correct_order(*lhs_itr, *rhs_itr);
        if (result == Result::Correct or result == Result::Incorrect)
        {
            return result;
        }
        ++rhs_itr;
        ++lhs_itr;
    }
    if (lhs_itr == lhs.end() and rhs_itr != rhs.end())
    {
        return Result::Correct;
    }
    if (lhs_itr != lhs.end() and rhs_itr == rhs.end())
    {
        return Result::Incorrect;
    }
    return Result::Unresolved;
}

constexpr auto packet_comparator = [](const List& lhs, const List& rhs) {
    return is_correct_order(lhs, rhs) == Result::Correct;
};

auto count_matching_pairs(utility::Stream& stream)
{
    auto result = 0;
    auto index = 1;
    while (stream.is_active())
    {
        const auto lhs = parse_input(stream.next_line());
        const auto rhs = parse_input(stream.next_line());
        if (packet_comparator(lhs, rhs))
        {
            result += index;
        }
        stream.next_line();
        ++index;
    }
    return result;
}

auto calculate_decoder_key(utility::Stream& stream)
{
    std::set<List, decltype(packet_comparator)> lists;
    for (const auto& line : stream)
    {
        if (line.empty())
        {
            continue;
        }
        lists.insert(parse_input(line));
    }

    auto result = 1;
    for (auto new_value : {2, 6})
    {
        const List divider{Element{List{Element{new_value}}}};
        const auto& [itr, _] = lists.insert(divider);
        const auto index = std::distance(lists.begin(), itr) + 1;
        result *= index;
    }
    return result;
}
} // namespace task
