#pragma once
#include "core/utils/time_utils.hpp"

#include <optional>

namespace utils
{
class DateFilter
{
  public:
    DateFilter(std::optional<TimePoint> start, std::optional<TimePoint> end);
    bool IsInRange(const TimePoint& tp) const;
    static DateFilter Create(std::optional<TimePoint> start, std::optional<TimePoint> end);
    static DateFilter NoFilter();

  private:
    std::optional<TimePoint> start_date_;
    std::optional<TimePoint> end_date_;
};
} // namespace utils
