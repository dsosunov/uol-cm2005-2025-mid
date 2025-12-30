#include "core/utils/time_utils.hpp"

#include <cctype>
#include <charconv>
#include <ctime>
#include <format>
#include <iomanip>
#include <sstream>

namespace utils
{
bool TimestampParser::IsDigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch)) != 0;
}

void TimestampParser::ConsumeWhitespace(std::string_view& sv)
{
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front())))
    {
        sv.remove_prefix(1);
    }
}

bool TimestampParser::ConsumeInt(std::string_view& sv, int& out)
{
    if (sv.empty() || !IsDigit(sv.front()))
    {
        return false;
    }

    const char* begin = sv.data();
    const char* end = sv.data() + sv.size();
    auto [ptr, ec] = std::from_chars(begin, end, out);
    if (ec != std::errc{})
    {
        return false;
    }

    sv.remove_prefix(static_cast<size_t>(ptr - begin));
    return true;
}

bool TimestampParser::ConsumeNonDigit(std::string_view& sv)
{
    if (sv.empty())
    {
        return false;
    }

    if (IsDigit(sv.front()))
    {
        return false;
    }

    sv.remove_prefix(1);
    return true;
}

int TimestampParser::ParseMicrosPadded6(std::string_view& sv)
{
    int value = 0;
    int digits = 0;
    while (digits < 6 && !sv.empty() && IsDigit(sv.front()))
    {
        value = (value * 10) + (sv.front() - '0');
        sv.remove_prefix(1);
        ++digits;
    }
    while (digits < 6)
    {
        value *= 10;
        ++digits;
    }
    return value;
}

void TimestampParser::TryParseTime(std::string_view sv, int& hour, int& minute, int& second,
                                   int& microseconds)
{
    // Original behavior: invalid/partial time => 00:00:00(.000000)
    hour = 0;
    minute = 0;
    second = 0;
    microseconds = 0;

    ConsumeWhitespace(sv);
    if (sv.empty() || !IsDigit(sv.front()))
    {
        return;
    }

    int h = 0;
    int m = 0;
    int s = 0;
    if (!ConsumeInt(sv, h) || sv.empty() || sv.front() != ':')
    {
        return;
    }
    sv.remove_prefix(1);

    if (!ConsumeInt(sv, m) || sv.empty() || sv.front() != ':')
    {
        return;
    }
    sv.remove_prefix(1);

    if (!ConsumeInt(sv, s))
    {
        return;
    }

    hour = h;
    minute = m;
    second = s;

    if (!sv.empty() && sv.front() == '.')
    {
        sv.remove_prefix(1);
        microseconds = ParseMicrosPadded6(sv);
    }
}

std::optional<TimePoint> TimestampParser::Parse(std::string_view str)
{
    if (str.empty())
    {
        return std::nullopt;
    }

    // Keep original “must contain '-' or '/' somewhere” gating.
    if (!str.contains('/') && !str.contains('-'))
    {
        return std::nullopt;
    }

    auto sv = str;

    // Parse date: YYYY{non-digit}MM{non-digit}DD
    int year = 0;
    int month = 0;
    int day = 0;
    if (!ConsumeInt(sv, year) || !ConsumeNonDigit(sv) || !ConsumeInt(sv, month) ||
        !ConsumeNonDigit(sv) || !ConsumeInt(sv, day))
    {
        return std::nullopt;
    }

    int hour = 0;
    int minute = 0;
    int second = 0;
    int microseconds = 0;
    TryParseTime(sv, hour, minute, second, microseconds);

    auto ymd = std::chrono::year{year} / std::chrono::month{static_cast<unsigned>(month)} /
               std::chrono::day{static_cast<unsigned>(day)};
    if (!ymd.ok())
    {
        return std::nullopt;
    }

    auto days_tp = std::chrono::sys_days{ymd};
    auto tod =
        std::chrono::hours{hour} + std::chrono::minutes{minute} + std::chrono::seconds{second};

    auto tp = std::chrono::time_point_cast<std::chrono::microseconds>(days_tp + tod);
    if (microseconds != 0)
    {
        tp += std::chrono::microseconds{microseconds};
    }
    return tp;
}

std::string FormatDate(const TimePoint& tp)
{
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm;

#ifdef _WIN32
    gmtime_s(&tm, &time);
#else
    gmtime_r(&time, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");

    return ss.str();
}

std::string FormatTimestamp(const TimePoint& tp)
{
    auto time = std::chrono::system_clock::to_time_t(tp);
    auto microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count() %
        1000000;
    std::tm tm;

#ifdef _WIN32
    gmtime_s(&tm, &time);
#else
    gmtime_r(&time, &tm);
#endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    ss << std::format(".{:06}", microseconds);

    return ss.str();
}
} // namespace utils