#pragma once

#include "utility/Dbg.hpp"
#include <fstream>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

namespace utility
{
using Arguments = std::vector<std::string>;
auto parse_arguments(int argc, char* argv[])
{
    Arguments arguments;
    for (auto i = 0; i < argc; ++i)
    {
        arguments.emplace_back(argv[i]);
    }
    return arguments;
}

auto get_input_filename(const Arguments& args)
{
    constexpr auto expected_arg_count = 2u;
    const auto args_count = args.size();
    if (args_count < expected_arg_count)
    {
        throw std::invalid_argument{"No input filename provided"};
    }
    const auto input_filename = args[1];
    if (not std::filesystem::exists(input_filename))
    {
        throw std::invalid_argument{CONCAT("Input file ", std::quoted(input_filename), " does not exist")};
    }
    return input_filename;
}

std::ifstream open_file(const std::string& filename)
{
    std::ifstream stream;
    stream.open(filename);
    if (not stream.is_open())
    {
        throw std::logic_error{CONCAT("File ", filename, " could not be open")};
    }
    return stream;
}
} // namespace utility
