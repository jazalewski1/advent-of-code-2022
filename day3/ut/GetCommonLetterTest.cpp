#include "Task.hpp"
#include "utility/Dbg.hpp"
#include <exception>
#include <string>
#include <gtest/gtest.h>

namespace
{
const std::string first_half_with_first_letter{"xaa"};
const std::string first_half_with_middle_letter{"axa"};
const std::string first_half_with_last_letter{"aax"};
const std::string second_half_with_first_letter{"xbb"};
const std::string second_half_with_middle_letter{"bxb"};
const std::string second_half_with_last_letter{"bbx"};
} // namespace

TEST(GetCommonLetterTest, Empty_input)
{
    const std::string input{""};
    EXPECT_THROW(task::get_common_letter(input), std::logic_error);
}

using Strings = std::tuple<std::string, std::string>;
struct GetCommonLetterTest_Mix : testing::TestWithParam<Strings> {};

TEST_P(GetCommonLetterTest_Mix, Get_common_letter)
{
    const auto& [first_half, second_half] = GetParam();
    const std::string input{first_half + second_half};
    const auto result = task::get_common_letter(input);
    EXPECT_EQ(result, 'x');
}

INSTANTIATE_TEST_SUITE_P(
    ,
    GetCommonLetterTest_Mix,
    testing::Combine(
        testing::Values(
            first_half_with_first_letter,
            first_half_with_middle_letter,
            first_half_with_last_letter),
        testing::Values(
            second_half_with_first_letter,
            second_half_with_middle_letter,
            second_half_with_last_letter)
    ),
    [](const auto& info) { return std::get<0>(info.param) + std::get<1>(info.param); }
);
