#include "core/utils/time_utils.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace utils
{
std::optional<TimePoint> ParseTimestamp(std::string_view str)
{
    if (str.empty())
    {
        return std::nullopt;
    }

    std::tm tm = {};
    std::istringstream ss{std::string(str)};

    int microseconds = 0;

    if (str.contains('/'))
    {
        char delim;
        ss >> tm.tm_year >> delim >> tm.tm_mon >> delim >> tm.tm_mday;
        if (ss.fail())
        {
            return std::nullopt;
        }
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;

        ss >> std::ws;
        if (std::isdigit(ss.peek()))
        {
            char time_delim;
            ss >> tm.tm_hour >> time_delim >> tm.tm_min >> time_delim >> tm.tm_sec;
            if (ss.fail())
            {
                tm.tm_hour = 0;
                tm.tm_min = 0;
                tm.tm_sec = 0;
            }

            if (ss.peek() == '.')
            {
                ss.get();
                std::string frac;
                frac.reserve(6);
                while (std::isdigit(ss.peek()) && frac.size() < 6)
                {
                    frac.push_back(static_cast<char>(ss.get()));
                }
                while (frac.size() < 6)
                {
                    frac.push_back('0');
                }

                try
                {
                    microseconds = std::stoi(frac);
                }
                catch (...)
                {
                    microseconds = 0;
                }
            }
        }
    }
    else if (str.contains('-'))
    {
        char delim;
        ss >> tm.tm_year >> delim >> tm.tm_mon >> delim >> tm.tm_mday;
        if (ss.fail())
        {
            return std::nullopt;
        }
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;

        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;

        ss >> std::ws;
        if (std::isdigit(ss.peek()))
        {
            char time_delim;
            ss >> tm.tm_hour >> time_delim >> tm.tm_min >> time_delim >> tm.tm_sec;
            if (ss.fail())
            {
                tm.tm_hour = 0;
                tm.tm_min = 0;
                tm.tm_sec = 0;
            }

            if (ss.peek() == '.')
            {
                ss.get();
                std::string frac;
                frac.reserve(6);
                while (std::isdigit(ss.peek()) && frac.size() < 6)
                {
                    frac.push_back(static_cast<char>(ss.get()));
                }
                while (frac.size() < 6)
                {
                    frac.push_back('0');
                }

                try
                {
                    microseconds = std::stoi(frac);
                }
                catch (...)
                {
                    microseconds = 0;
                }
            }
        }
    }
    else
    {
        return std::nullopt;
    }

    tm.tm_isdst = -1;

    auto ymd = std::chrono::year{tm.tm_year + 1900} /
               std::chrono::month{static_cast<unsigned>(tm.tm_mon + 1)} /
               std::chrono::day{static_cast<unsigned>(tm.tm_mday)};
    auto hms = std::chrono::hours{tm.tm_hour} + std::chrono::minutes{tm.tm_min} +
               std::chrono::seconds{tm.tm_sec};
    auto days = std::chrono::sys_days{ymd};

    if (!ymd.ok())
    {
        return std::nullopt;
    }

    auto tp = std::chrono::time_point_cast<std::chrono::microseconds>(days + hms);
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