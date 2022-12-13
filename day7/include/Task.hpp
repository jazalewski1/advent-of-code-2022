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

enum class Destination { Root, Parent, NewDirectory };
struct ChangeDirectory
{
    Destination destination;
};

struct InspectFile
{
    Size size;
};

using Command = std::variant<ChangeDirectory, InspectFile>;

Destination parse_destination(const std::string& directory)
{
    constexpr auto root = "/";
    constexpr auto parent = "..";
    if (directory == root)
    {
        return Destination::Root;
    }
    if (directory == parent)
    {
        return Destination::Parent;
    }
    return Destination::NewDirectory;
}

std::optional<Command> parse_command(const std::string& line)
{
    const std::regex change_directory_regex{R"((\$ cd )(.*))"};
    if (std::smatch result; std::regex_search(line, result, change_directory_regex))
    {
        const auto destination = parse_destination(result.str(2));
        return Command{ChangeDirectory{destination}};
    }
    const std::regex file_regex{R"(^(\d+)( .*))"};
    if (std::smatch result; std::regex_search(line, result, file_regex))
    {
        const Size size{std::stoull(result.str(1))};
        return Command{InspectFile{size}};
    }
    return std::nullopt;
}

class Accumulator
{
public:
    Accumulator(Size bound) : directory_size_bound{bound}
    {
    }

    void execute(const Command& command)
    {
        std::visit([this](const auto& type) { execute(type); }, command);
    }

    Size get_sum_of_sizes() const { return sum_of_sizes_below_bound; }

    Size find_smallest_directory_to_remove() const
    {
        constexpr Size required_size = 30'000'000;
        constexpr Size total_size = 70'000'000;
        const Size space_left = total_size - used_size;
        if (space_left > required_size)
        {
            return Size{0};
        }
        constexpr Size max_available_size = total_size - required_size;
        const Size needed_to_remove = used_size - max_available_size;
        const auto to_remove = std::ranges::upper_bound(sorted_sizes, needed_to_remove);
        if (to_remove != sorted_sizes.end())
        {
            return *to_remove;
        }
        return Size{0};
    }

private:
    std::vector<Size> stack_of_sizes;
    std::set<Size> sorted_sizes;
    Size current_dir_size{0};
    Size used_size{0};
    const Size directory_size_bound;
    Size sum_of_sizes_below_bound{0};

    void execute(const ChangeDirectory& command)
    {
        if (command.destination == Destination::Root)
        {
            stack_of_sizes.push_back(Size{});
        }
        else if (command.destination == Destination::Parent)
        {
            sorted_sizes.insert(current_dir_size);
            if (current_dir_size < 100'000)
            {
                sum_of_sizes_below_bound += current_dir_size;
            }
            current_dir_size += stack_of_sizes.back();
            stack_of_sizes.pop_back();
        }
        else
        {
            stack_of_sizes.push_back(current_dir_size);
            current_dir_size = Size{};
        }
    }

    void execute(const InspectFile& file)
    {
        current_dir_size += file.size;
        used_size += file.size;
    }
};
} // namespace system

using UpperBound = system::Size;
auto accumulate_directory_sizes(utility::Stream& stream, UpperBound upper_bound)
{
    system::Accumulator accumulator{upper_bound};
    for (const auto& line : stream)
    {
        const auto command = system::parse_command(line);
        if (command.has_value())
        {
            accumulator.execute(command.value());
        }
    }
    return accumulator.get_sum_of_sizes();
}

auto find_smallest_to_remove(utility::Stream& stream)
{
    system::Accumulator accumulator{UpperBound{}};
    for (const auto& line : stream)
    {
        const auto command = system::parse_command(line);
        if (command.has_value())
        {
            accumulator.execute(command.value());
        }
    }
    return accumulator.find_smallest_directory_to_remove();
}
} // namespace task
