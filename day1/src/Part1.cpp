#include "Task.hpp"
#include "utility/Arguments.hpp"
#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <optional>

namespace
{
auto execute(utility::Stream& stream)
{
    return task::accumulate_largest_values(stream, 1);
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
