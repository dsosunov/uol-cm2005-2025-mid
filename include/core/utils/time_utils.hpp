#pragma once
#include <chrono>
#include <optional>
#include <string>
#include <string_view>
namespace utils
{
using TimePoint = std::chrono::system_clock::time_point;

class TimestampParser final
{
  public:
    static std::optional<TimePoint> Parse(std::string_view str);

  private:
    static bool IsDigit(char ch);
    static void ConsumeWhitespace(std::string_view& sv);
    static bool ConsumeInt(std::string_view& sv, int& out);
    static bool ConsumeNonDigit(std::string_view& sv);
    static int ParseMicrosPadded6(std::string_view& sv);
    static void TryParseTime(std::string_view sv, int& hour, int& minute, int& second,
                             int& microseconds);
};
std::string FormatDate(const TimePoint& tp);
std::string FormatTimestamp(const TimePoint& tp);
inline TimePoint Now()
{
    return std::chrono::system_clock::now();
}
} // namespace utils