#include "core/utils/date_filter.hpp"
namespace utils
{
DateFilter::DateFilter(std::optional<TimePoint> start, std::optional<TimePoint> end)
    : start_date_(start), end_date_(end)
{
}

bool DateFilter::IsInRange(const TimePoint& tp) const
{
    if (start_date_.has_value() && tp < *start_date_)
    {
        return false;
    }

    if (end_date_.has_value())
    {
        auto end_check = *end_date_;

        // Check if start equals end and both are at midnight
        if (start_date_.has_value() && *start_date_ == *end_date_)
        {
            auto duration = end_check.time_since_epoch();
            auto days = std::chrono::duration_cast<std::chrono::days>(duration);
            auto time_of_day = duration - days;

            if (time_of_day == std::chrono::seconds(0))
            {
                // If start equals end at midnight, include the entire day
                end_check += std::chrono::hours(24);
            }
        }

        if (tp >= end_check)
        {
            return false;
        }
    }

    return true;
}

DateFilter DateFilter::Create(std::optional<TimePoint> start, std::optional<TimePoint> end)
{
    return DateFilter(start, end);
}

DateFilter DateFilter::NoFilter()
{
    return DateFilter(std::nullopt, std::nullopt);
}
} // namespace utils
