#include "Task.hpp"
#include "utility/Arguments.hpp"
#include "utility/Stream.hpp"
#include "utility/Dbg.hpp"

namespace
{
auto execute(utility::Stream& stream)
{
    auto result = 0;
    while (stream.is_active())
    {
        const auto line1 = stream.next_line();
        const auto line2 = stream.next_line();
        const auto line3 = stream.next_line();
        const auto common_letter = task::get_common_letter(line1, line2, line3);
        result += task::convert_to_priority(common_letter);
    }
    return result;
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
