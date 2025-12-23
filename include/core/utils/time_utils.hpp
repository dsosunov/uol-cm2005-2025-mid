#pragma once
#include <chrono>
#include <optional>
#include <string>
#include <string_view>
namespace utils
{
    using TimePoint = std::chrono::system_clock::time_point;
    std::optional<TimePoint> ParseTimestamp(std::string_view str);
    std::string FormatDate(const TimePoint &tp);
    std::string FormatTimestamp(const TimePoint &tp);
    inline TimePoint Now()
    {
        return std::chrono::system_clock::now();
    }
}