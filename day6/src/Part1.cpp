#include "Task.hpp"
#include "utility/Arguments.hpp"
#include "utility/Stream.hpp"
#include "utility/Dbg.hpp"

namespace
{
auto execute(utility::Stream& stream)
{
    const auto line = stream.next_line();
    constexpr auto marker_length = 4u;
    return task::count_characters_to_marker_end(line, marker_length);
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
