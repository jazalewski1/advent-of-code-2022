#include "Task.hpp"
#include "utility/Arguments.hpp"
#include "utility/Stream.hpp"
#include "utility/Dbg.hpp"

namespace
{
auto execute(utility::Stream& stream)
{
    constexpr auto row = 2'000'000;
    // constexpr auto row = 10;
    return task::count_empty_positions(stream, row);
}
} // namespace

int main(int argc, char* argv[])
{
    try
    {
        const auto args = utility::parse_arguments(argc, argv);
        const auto input_filename = utility::get_input_filename(args);
        auto input_stream = utility::open_file(input_filename);
        utility::Stream stream{input_stream};
        const auto answer = execute(stream);
        DBG(answer);
    }
    catch(const std::exception& e)
    {
        LOG("Exception: ", e.what());
        return 1;
    }

    return 0;
}
