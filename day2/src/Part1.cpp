#include "utility/Arguments.hpp"
#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"

namespace
{
auto get_score(char opponent_symbol, char player_symbol)
{
    const auto opponent_move = static_cast<int>(opponent_symbol);
    const auto player_move = static_cast<int>(player_symbol);

    const auto modificator_for_opponents_move = (-opponent_move % 3) - 1;
    const auto score_for_result = 3 * ((player_move + modificator_for_opponents_move) % 3);
    const auto score_for_move = ((player_move - 1) % 3) + 1;
    return score_for_result + score_for_move;
}

auto execute(utility::Stream& stream)
{
    auto score = 0;
    for (const auto& line : stream)
    {
        const auto opponent_symbol = line.at(0);
        const auto player_symbol = line.at(2);
        score += get_score(opponent_symbol, player_symbol);
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
