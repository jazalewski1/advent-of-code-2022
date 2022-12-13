#pragma once

#include "utility/Dbg.hpp"
#include "utility/Stream.hpp"
#include <numeric>
#include <set>
#include <vector>

namespace task
{
using Batch = unsigned;
using SortedBatches = std::set<Batch>;

auto parse_meal_list(utility::Stream& stream)
{
    SortedBatches out;
    for (auto stream_itr = stream.begin(); stream_itr != stream.end(); ++stream_itr)
    {
        Batch batch{0};
        for ( ; stream_itr != stream.end() and not stream_itr->empty(); ++stream_itr)
        {
            batch += std::stoi(*stream_itr);
        }
        out.insert(batch);
    }
    return out;
}

Batch accumulate_largest_values(utility::Stream& stream, unsigned count)
{
    const auto sorted_batches = parse_meal_list(stream);
    return std::accumulate(sorted_batches.rbegin(), std::next(sorted_batches.rbegin(), count), Batch{});
}
} // namespace task
