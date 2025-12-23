#include "core/utils/date_filter.hpp"
namespace utils
{
    DateFilter::DateFilter(std::optional<TimePoint> start, std::optional<TimePoint> end)
        : start_date_(start), end_date_(end)
    {
    }
    bool DateFilter::IsInRange(const TimePoint &tp) const
    {
        if (start_date_.has_value())
        {
            std::string tp_str = FormatDate(tp);
            std::string start_str = FormatDate(*start_date_);
            if (tp_str < start_str)
                return false;
        }
        if (end_date_.has_value())
        {
            std::string tp_str = FormatDate(tp);
            std::string end_str = FormatDate(*end_date_);
            if (tp_str > end_str)
                return false;
        }
        return true;
    }
    bool DateFilter::IsInRange(std::string_view date_string) const
    {
        if (start_date_.has_value())
        {
            std::string start_str = FormatDate(*start_date_);
            if (date_string < start_str)
                return false;
        }
        if (end_date_.has_value())
        {
            std::string end_str = FormatDate(*end_date_);
            if (date_string > end_str)
                return false;
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
}
