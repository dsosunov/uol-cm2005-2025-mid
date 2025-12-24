#pragma once
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

#include <optional>
#include <string>

namespace dto
{
struct ActivitySummary
{
    Timeframe timeframe;
    std::optional<utils::TimePoint> start_date;
    std::optional<utils::TimePoint> end_date;
};
} // namespace dto