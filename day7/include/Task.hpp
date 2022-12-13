#pragma once

#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <variant>
#include <vector>

namespace task
{
namespace system
{
using Size = unsigned long long;

struct GoForward {};
struct GoBackward {};
struct InspectFile { Size size; };
using Command = std::variant<GoForward, GoBackward, InspectFile>;

std::optional<Command> parse_command(const std::string& line)
{
    const std::regex change_directory_regex{R"((\$ cd )(.*))"};
    if (std::smatch result; std::regex_search(line, result, change_directory_regex))
    {
        return result.str(2) == ".." ? Command{GoBackward{}} : Command{GoForward{}};
    }
    const std::regex file_regex{R"(^(\d+)( .*))"};
    if (std::smatch result; std::regex_search(line, result, file_regex))
    {
        const Size size{std::stoull(result.str(1))};
        return Command{InspectFile{size}};
    }
    return std::nullopt;
}

class Stack
{
public:
    void push() { stack.push_back(std::exchange(current_dir, Size{0})); }

    void pop()
    {
        current_dir += stack.back();
        stack.pop_back();
    }

    void add_file(Size size) { current_dir += size; }

    Size get_current() const { return current_dir; }

private:
    std::vector<Size> stack;
    Size current_dir;
};

class BoundedAccumulator
{
public:
    void execute(const Command& command)
    {
        std::visit([this](const auto& type) { execute(type); }, command);
    }

    Size get_result() const
    {
        return sum_of_sizes_below_bound;
    }

private:
    Size sum_of_sizes_below_bound{0};
    const Size upper_bound{100'000};
    Stack stack;

    void execute(const GoForward&)
    {
        stack.push();
    }

    void execute(const GoBackward&)
    {
        if (const auto current_size = stack.get_current(); current_size < upper_bound)
        {
            sum_of_sizes_below_bound += current_size;
        }
        stack.pop();
    }

    void execute(const InspectFile& file)
    {
        stack.add_file(file.size);
    }
};

class SortedAccumulator
{
public:
    void execute(const Command& command)
    {
        std::visit([this](const auto& type) { execute(type); }, command);
    }

    Size get_result() const
    {
        constexpr Size required_space = 30'000'000;
        constexpr Size total_space = 70'000'000;
        constexpr Size max_available_space = total_space - required_space;
        const Size space_to_remove = used_space - max_available_space;
        const auto to_remove = std::ranges::upper_bound(sorted_sizes, space_to_remove);
        if (to_remove != sorted_sizes.end())
        {
            return *to_remove;
        }
        return Size{0};
    }

private:
    std::set<Size> sorted_sizes;
    Stack stack;
    Size used_space;

    void execute(const GoForward&)
    {
        stack.push();
    }

    void execute(const GoBackward&)
    {
        sorted_sizes.insert(stack.get_current());
        stack.pop();
    }

    void execute(const InspectFile& file)
    {
        used_space += file.size;
        stack.add_file(file.size);
    }
};
} // namespace system

using UpperBound = system::Size;
auto accumulate_directory_sizes(utility::Stream& stream)
{
    system::BoundedAccumulator accumulator{};
    for (const auto& line : stream)
    {
        const auto command = system::parse_command(line);
        if (command.has_value())
        {
            accumulator.execute(command.value());
        }
    }
    return accumulator.get_result();
}

auto find_smallest_to_remove(utility::Stream& stream)
{
    system::SortedAccumulator accumulator{};
    for (const auto& line : stream)
    {
        const auto command = system::parse_command(line);
        if (command.has_value())
        {
            accumulator.execute(command.value());
        }
    }
    return accumulator.get_result();
}
} // namespace task
