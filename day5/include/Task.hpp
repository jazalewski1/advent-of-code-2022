#pragma once

#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <algorithm>
#include <regex>
#include <string>
#include <vector>

namespace task
{
using Strings = std::vector<std::string>;
using Stack = std::vector<char>;
using Stacks = std::vector<Stack>;

struct Instruction
{
    unsigned quantity;
    unsigned source;
    unsigned target;
};
using Instructions = std::vector<Instruction>;

auto is_upper_letter(char c) { return c >= 'A' and c <= 'Z'; }

auto parse_stacks(utility::Stream& stream)
{
    constexpr auto first_bracket_offset = 1u;
    Stacks out;
    for (const auto& line : stream)
    {
        if (line.empty())
        {
            break;
        }
        auto symbol_itr = std::find_if(line.begin(), line.end(), is_upper_letter);
        while (symbol_itr != line.end())
        {
            const auto distance_from_begin = std::distance(line.begin() + first_bracket_offset, symbol_itr);
            const unsigned stack_index = distance_from_begin / 4u;
            if (stack_index >= out.size())
            {
                out.resize(stack_index + 1);
            }
            auto& stack = out.at(stack_index);
            stack.insert(stack.begin(), *symbol_itr);

            symbol_itr = std::find_if(symbol_itr + 1, line.end(), is_upper_letter);
        }
    }
    return out;
}

auto parse_instructions(utility::Stream& stream)
{
    Instructions out;
    const std::regex regex{R"((move )(\d+)( from )(\d+)( to )(\d+))"};
    for (const auto& line : stream)
    {
        std::smatch result;
        std::regex_search(line, result, regex);
        out.emplace_back(std::stoi(result.str(2)), std::stoi(result.str(4)) - 1, std::stoi(result.str(6)) - 1);
    }
    return out;
}

struct KeepOrder {};
struct StackOrder {};

void move_crates(Stack& source, Stack& target, const Instruction& instruction, StackOrder)
{
    const auto cut_itr = source.rbegin() + instruction.quantity;
    target.reserve(target.size() + instruction.quantity);
    std::move(source.rbegin(), cut_itr, std::back_inserter(target));
    source.erase(source.end() - instruction.quantity, source.end());
}

void move_crates(Stack& source, Stack& target, const Instruction& instruction, KeepOrder)
{
    const auto cut_itr = source.end() - instruction.quantity;
    target.reserve(target.size() + instruction.quantity);
    std::move(cut_itr, source.end(), std::back_inserter(target));
    source.erase(cut_itr, source.end());
}

template <typename OrderingTag>
void perform(Stacks& stacks, const Instructions& instructions, OrderingTag ordering_tag)
{
    for (const auto& instruction : instructions)
    {
        auto& source = stacks.at(instruction.source);
        auto& target = stacks.at(instruction.target);
        move_crates(source, target, instruction, ordering_tag);
    }
}

auto accumulate_tops(const Stacks& stacks)
{
    std::string out;
    for (const auto& stack : stacks)
    {
        out.push_back(stack.back());
    }
    return out;
}

template <typename OrderingTag>
std::string get_top_crates(utility::Stream& stream, OrderingTag ordering_tag)
{
    auto stacks = parse_stacks(stream);
    const auto instructions = parse_instructions(stream);
    perform(stacks, instructions, ordering_tag);
	return accumulate_tops(stacks);
}
} // namespace task
