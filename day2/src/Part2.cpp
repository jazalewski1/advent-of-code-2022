#include "utility/Arguments.hpp"
#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <exception>
#include <string>

namespace
{
auto calculate_score(char opponent_symbol, char result_symbol)
{
    const auto result = static_cast<int>(result_symbol);
    const auto opponent = static_cast<int>(opponent_symbol);
    auto move_modificator_by_result = (result % 3) - 1;
    auto score_for_move = ((opponent + move_modificator_by_result) % 3) + 1;
    auto score_for_result = 3 * ((result - 1) % 3);
    return score_for_move + score_for_result;
}

auto execute(utility::Stream& stream)
{
    auto score = 0u;
    for (const auto& line : stream)
    {
        const auto opponent_move_symbol = line.at(0);
        const auto result_symbol = line.at(2);
        score += calculate_score(opponent_move_symbol, result_symbol);
    }
    return score;
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
