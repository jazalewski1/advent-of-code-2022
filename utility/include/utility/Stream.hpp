#pragma once

#include "utility/Dbg.hpp"
#include <exception>
#include <fstream>
#include <iomanip>
#include <optional>
#include <string>
#include <iterator>
#include <istream>

namespace utility
{
class StreamIterator
{
public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::string;

    StreamIterator(std::istream& s) : stream{&s}
    {
        read();
    }

    StreamIterator() : stream{nullptr}, is_end_of_stream{true} {}

    const value_type& operator*() const { return current_line; }

    const value_type* operator->() const { return &current_line; }

    StreamIterator& operator++()
    {
        read();
        return *this;
    }

    StreamIterator operator++(int) { return StreamIterator{*stream}; }

    bool operator==(const StreamIterator& other) const
    {
        if (this->is_end_of_stream and other.is_end_of_stream)
        {
            return true;
        }
        return this->stream == other.stream;
    }

    bool operator!=(const StreamIterator& other) const { return not(other == *this); }

private:
    std::istream* stream;
    std::string current_line;
    bool is_end_of_stream;

    void read()
    {
        is_end_of_stream = stream->eof();
        if (not is_end_of_stream)
        {
            std::getline(*stream, current_line);
        }
    }
};

class Stream
{
public:
    Stream(std::istream& stream) : stream{stream}
    {
    }

    bool is_active() const { return not stream.eof(); }

    std::optional<std::string> next_line() { return *StreamIterator{stream}; }

    StreamIterator begin() { return StreamIterator{stream}; }

    StreamIterator end() { return StreamIterator{}; }

private:
    std::istream& stream;
};
} // namespace utility
