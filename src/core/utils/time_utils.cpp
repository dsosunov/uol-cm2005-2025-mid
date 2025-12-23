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
        if (str.find('/') != std::string_view::npos)
        {
            char delim;
            ss >> tm.tm_year >> delim >> tm.tm_mon >> delim >> tm.tm_mday;
            if (ss.fail())
            {
                return std::nullopt;
            }
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            if (!ss.eof())
            {
                ss >> tm.tm_hour >> delim >> tm.tm_min >> delim >> tm.tm_sec;
                if (ss.fail())
                {
                    tm.tm_hour = 0;
                    tm.tm_min = 0;
                    tm.tm_sec = 0;
                }
            }
        }
        else if (str.find('-') != std::string_view::npos)
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
        }
        else
        {
            return std::nullopt;
        }
        tm.tm_isdst = -1;
        std::time_t time = std::mktime(&tm);
        if (time == -1)
        {
            return std::nullopt;
        }
        return std::chrono::system_clock::from_time_t(time);
    }
    std::string FormatDate(const TimePoint &tp)
    {
        auto time = std::chrono::system_clock::to_time_t(tp);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &time);
#else
        localtime_r(&time, &tm);
#endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d");
        return ss.str();
    }
    std::string FormatTimestamp(const TimePoint &tp)
    {
        auto time = std::chrono::system_clock::to_time_t(tp);
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                                tp.time_since_epoch())
                                .count() %
                            1000000;
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &time);
#else
        localtime_r(&time, &tm);
#endif
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(6) << microseconds;
        return ss.str();
    }
}