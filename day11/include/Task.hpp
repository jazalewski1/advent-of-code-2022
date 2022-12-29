#pragma once

#include "utility/Stream.hpp"
#include <functional>
#include <numeric>
#include <regex>
#include <set>
#include <vector>
#include <ranges>

namespace task
{
using Item = unsigned long long;
using Items = std::vector<Item>;
struct Operation
{
    enum class Type {Addition, Multiplication} type;
    std::optional<Item> item;
};
using InspectionCount = unsigned long long;
struct Monkey
{
    Items items;
    Operation operation;
    int modulus;
    unsigned true_recipient;
    unsigned false_recipient;
    InspectionCount inspection_count{0};
};
using MonkeyList = std::vector<Monkey>;

Items parse_items(std::string string)
{
    Items out;
    const std::regex regex{R"(\d+)"};
    std::smatch result;
    while (std::regex_search(string, result, regex))
    {
        const auto value = std::stoi(result.str());
        out.push_back(value);
        string = result.suffix();
    }
    return out;
}

Operation parse_operation(const std::string& string)
{
    const std::regex regex{R"((.* new = old)( )([*+])( )(old|\d+))"};
    std::smatch result;
    std::regex_search(string, result, regex);

    const auto operator_symbol = result.str(3).at(0);
    const auto operation_type = operator_symbol == '*' ? Operation::Type::Multiplication : Operation::Type::Addition;
    const auto rhs_str = result.str(5);
    const std::optional<Item> value = (rhs_str == "old" ? std::optional<Item>{} : std::stoi(rhs_str));
    return Operation{operation_type, value};
}

int parse_test(const std::string& string)
{
    const std::regex regex{R"(\d+)"};
    std::smatch result;
    std::regex_search(string, result, regex);
    return std::stoi(result.str());
}

unsigned parse_recipient(const std::string& string)
{
    const std::regex regex{R"(\d+)"};
    std::smatch result;
    std::regex_search(string, result, regex);
    return std::stoi(result.str());
}

Monkey parse_monkey(utility::Stream& stream)
{
    Monkey monkey;

    stream.next_line();
    monkey.items = parse_items(stream.next_line());
    monkey.operation = parse_operation(stream.next_line());
    monkey.modulus = parse_test(stream.next_line());
    monkey.true_recipient = parse_recipient(stream.next_line());
    monkey.false_recipient = parse_recipient(stream.next_line());

    return monkey;
}

MonkeyList parse_input(utility::Stream& stream)
{
    MonkeyList monkeys;
    while (stream.is_active())
    {
        monkeys.push_back(parse_monkey(stream));
        stream.next_line();
    }
    return monkeys;
}

Item calculate(Operation operation, Item item)
{
    if (operation.type == Operation::Type::Addition)
    {
        return item + operation.item.value_or(item);
    }
    return item * operation.item.value_or(item);
}

using WorryModifier = std::function<Item (Item)>;
void perform_turn(Monkey& monkey, MonkeyList& monkeys, WorryModifier&& modifier)
{
    for (auto item : monkey.items)
    {
        item = modifier(calculate(monkey.operation, item));
        const auto recipient = item % monkey.modulus == 0 ? monkey.true_recipient : monkey.false_recipient;
        monkeys.at(recipient).items.push_back(item);
        ++monkey.inspection_count;
    }
    monkey.items.clear();
}

auto count_highest_score(const MonkeyList& monkeys)
{
    std::set<InspectionCount> sorted;
    for (const auto& monkey : monkeys)
    {
        sorted.insert(monkey.inspection_count);
    }
    return *sorted.rbegin() * *std::next(sorted.rbegin());
}

auto accumulate_modulos(const MonkeyList& monkeys)
{
    const auto multiply_by_modulus = [](unsigned r, const auto& m) { return r * m.modulus; };
    return std::accumulate(monkeys.begin(), monkeys.end(), unsigned{1}, multiply_by_modulus);
}

void execute_rounds(MonkeyList& monkeys, unsigned num_of_rounds, WorryModifier&& modifier)
{
    for (auto i = 1u; i <= num_of_rounds; ++i)
    {
        for (auto& monkey : monkeys)
        {
            perform_turn(monkey, monkeys, std::move(modifier));
        }
    }
}

auto count_monkey_business(utility::Stream& stream, unsigned num_of_rounds)
{
    auto monkeys = parse_input(stream);
    execute_rounds(monkeys, num_of_rounds, [](Item i) { return i / 3; });
    return count_highest_score(monkeys);
}

auto count_hard_monkey_business(utility::Stream& stream, unsigned num_of_rounds)
{
    auto monkeys = parse_input(stream);
    const auto accumulated_modulos = accumulate_modulos(monkeys);
    execute_rounds(monkeys, num_of_rounds, [accumulated_modulos](Item i) { return i % accumulated_modulos; });
    return count_highest_score(monkeys);
}
} // namespace task
