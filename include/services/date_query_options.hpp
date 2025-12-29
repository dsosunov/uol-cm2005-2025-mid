#pragma once

#include "core/utils/time_utils.hpp"

#include <optional>

namespace services
{
struct DateQueryOptions
{
    std::optional<utils::TimePoint> start_date;
    std::optional<utils::TimePoint> end_date;
    std::optional<int> limit;
    int offset = 0;
};
} // namespace services
