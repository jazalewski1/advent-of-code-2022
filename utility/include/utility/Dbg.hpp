#pragma once

#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
#include <string>

auto get_filename(std::string s)
{
    const auto last_slash_pos = s.find_last_of('/');
    if (last_slash_pos != std::string::npos)
    {
        s.erase(0, last_slash_pos + 1);
    }
    return s;
}

#define LOCATION std::string{"["}.append(get_filename(__FILE__)).append(":").append(std::to_string(__LINE__)).append("]")

#define LOG_MACRO(arg) std::cout << LOCATION << ' ' << arg << std::endl;

template <typename T>
void print(std::ostream& stream, const T& data)
{
    stream << data;
}

template <>
void print(std::ostream& stream, const char& c)
{
    stream << '\'' << c << '\'';
}

template <typename T, typename U>
void print(std::ostream& stream, const std::pair<T, U>& pair)
{
    stream << '{';
    print(stream, pair.first);
    stream << ", ";
    print(stream, pair.second);
    stream << '}';
}

template <typename ContainedType>
void print(std::ostream& stream, const std::stack<ContainedType>& stack)
{
	stream << "stack:{";
	if (not stack.empty())
	{
        stream << "size = ";
		print(stream, stack.size());
		stream << ", top = ";
		print(stream, stack.top());
	}
    stream << '}';
}

template <typename T>
concept Stringable = requires (T data)
{
    {data} -> std::convertible_to<std::string>;
    typename T::iterator;
};

template <Stringable String>
void print(std::ostream& stream, const String& string)
{
    stream << std::quoted(string);
}

template <typename T>
concept Iterable = requires
{
    typename T::iterator;
};

template <Iterable Container> requires (not Stringable<Container>)
static void print(std::ostream& stream, const Container& container)
{
    stream << '[';
    if (not container.empty())
    {
        const auto first_element_iter = container.begin();
        const auto size = container.size();
        const auto last_element_iter = std::next(first_element_iter, size - 1);
        std::for_each(first_element_iter, last_element_iter, [&stream](const auto& e) {
            print(stream, e);
            stream << ", ";
        });
        print(stream, *last_element_iter);
    }
    stream << ']';
}

#define DBG(expr) \
    [&](auto&& e) -> decltype(auto) { \
        using Expr = decltype(e); \
        std::stringstream stream; \
        stream << std::boolalpha; \
        stream << #expr << ':'; \
        print(stream, e); \
        stream << std::noboolalpha; \
        LOG_MACRO(stream.str()); \
        return std::forward<Expr>(e); \
    } (expr)

struct Stringified
{
    std::string string;
};

std::ostream& operator<<(std::ostream& stream, const Stringified& in) { return stream << in.string; }

#define STR(expr) \
    [&](auto&& e) -> decltype(auto) { \
        std::stringstream stream; \
        stream << #expr << ':'; \
        stream << std::boolalpha; \
        print(stream, e); \
        stream << std::noboolalpha; \
        return Stringified{stream.str()}; \
    } (expr)

template <typename ... Ts>
void log(const std::string& location, const Ts& ... data)
{
    std::cout << location << ' ';
    ((print(std::cout, data)), ...);
    std::cout << std::endl;
}

#define LOG(...) log(LOCATION, __VA_ARGS__)

template <typename ... Ts>
std::string concatenate(const Ts& ... data)
{
    std::stringstream stream;
    ((print(stream, data)), ...);
    return stream.str();
}

#define CONCAT(...) concatenate(__VA_ARGS__)

#define LOG_SEPARATOR() log(LOCATION, "----------------------------------------------------------------------")
