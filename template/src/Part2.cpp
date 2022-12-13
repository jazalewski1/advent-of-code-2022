#include "Task.hpp"
#include "utility/Arguments.hpp"
#include "utility/Stream.hpp"
#include "utility/Dbg.hpp"

namespace
{
auto execute(utility::Stream& stream)
{
    return 0;
}
} // namespace

int main(int argc, char* argv[])
{
    const auto args = utility::parse_arguments(argc, argv);

    std::string input_filename;
    utility::Stream stream;
    try
    {
        input_filename = utility::get_input_filename(args);
        stream.open(input_filename);
    }
    catch(const std::exception& e)
    {
        LOG("Exception: ", e.what());
        return 1;
    }

    const auto answer = execute(stream);
    DBG(answer);

    return 0;
}

