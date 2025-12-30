#pragma once
#include "core/ui/form/form_context.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

#include <algorithm>
#include <any>
#include <cctype>
#include <charconv>
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

namespace form
{

class PeriodDateParser final
{
  public:
    static std::optional<utils::TimePoint> ParsePeriodStart(dto::Timeframe timeframe,
                                                            std::string_view text)
    {
        if (timeframe == dto::Timeframe::Yearly)
        {
            auto y = ParseYear(text);
            if (!y.has_value())
            {
                return std::nullopt;
            }
            auto ymd = *y / std::chrono::January / std::chrono::day{1};
            auto days = std::chrono::sys_days{ymd};
            return std::chrono::time_point_cast<std::chrono::microseconds>(days);
        }

        if (timeframe == dto::Timeframe::Monthly)
        {
            auto ym = ParseYearMonth(text);
            if (!ym.has_value())
            {
                return std::nullopt;
            }

            auto ymd = ym->first / ym->second / std::chrono::day{1};
            auto days = std::chrono::sys_days{ymd};
            return std::chrono::time_point_cast<std::chrono::microseconds>(days);
        }

        // Daily (or unknown): fall back to the existing timestamp parser.
        return utils::TimestampParser::Parse(text);
    }

    static std::optional<utils::TimePoint> ParsePeriodEndExclusive(dto::Timeframe timeframe,
                                                                   std::string_view text)
    {
        if (timeframe == dto::Timeframe::Yearly)
        {
            auto y = ParseYear(text);
            if (!y.has_value())
            {
                return std::nullopt;
            }

            auto end = (*y + std::chrono::years{1}) / std::chrono::January / std::chrono::day{1};
            auto end_days = std::chrono::sys_days{end};
            return std::chrono::time_point_cast<std::chrono::microseconds>(end_days);
        }

        if (timeframe == dto::Timeframe::Monthly)
        {
            auto ym = ParseYearMonth(text);
            if (!ym.has_value())
            {
                return std::nullopt;
            }

            auto ymd = ym->first / ym->second / std::chrono::day{1};
            auto next = std::chrono::year_month_day{ymd} + std::chrono::months{1};
            auto end_days = std::chrono::sys_days{next};
            return std::chrono::time_point_cast<std::chrono::microseconds>(end_days);
        }

        // Daily (or unknown): interpret the selected day as inclusive, so end is next day.
        if (auto day_start = utils::TimestampParser::Parse(text); day_start.has_value())
        {
            return *day_start + std::chrono::days{1};
        }

        return std::nullopt;
    }

  private:
    static std::string_view TrimWhitespace(std::string_view text)
    {
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
        {
            text.remove_prefix(1);
        }
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
        {
            text.remove_suffix(1);
        }
        return text;
    }

    static std::optional<int> ParseInt(std::string_view text)
    {
        text = TrimWhitespace(text);
        if (text.empty())
        {
            return std::nullopt;
        }

        int value = 0;
        const char* begin = text.data();
        const char* end = text.data() + text.size();
        auto [ptr, ec] = std::from_chars(begin, end, value);
        if (ec != std::errc{} || ptr != end)
        {
            return std::nullopt;
        }
        return value;
    }

    static std::optional<std::chrono::year> ParseYear(std::string_view text)
    {
        text = TrimWhitespace(text);
        if (text.size() != 4)
        {
            return std::nullopt;
        }

        auto year_int = ParseInt(text);
        if (!year_int.has_value())
        {
            return std::nullopt;
        }

        std::chrono::year y{*year_int};
        if (!y.ok())
        {
            return std::nullopt;
        }

        return y;
    }

    static std::optional<std::pair<std::chrono::year, std::chrono::month>> ParseYearMonth(
        std::string_view text)
    {
        text = TrimWhitespace(text);
        if (text.size() != 7 || text[4] != '-')
        {
            return std::nullopt;
        }

        auto y = ParseYear(text.substr(0, 4));
        auto m_int = ParseInt(text.substr(5, 2));
        if (!y.has_value() || !m_int.has_value())
        {
            return std::nullopt;
        }

        std::chrono::month m{static_cast<unsigned>(*m_int)};
        if (!m.ok())
        {
            return std::nullopt;
        }

        return std::pair{*y, m};
    }
};

template <typename T, typename FieldType> class SimpleFieldBinder
{
  public:
    using MemberPtr = FieldType T::*;
    explicit SimpleFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }
    void operator()(std::any& target, const std::any& value, const FormContext&) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();
        if constexpr (std::is_same_v<FieldType, std::string>)
        {
            obj.*member_ptr_ = std::any_cast<std::string>(value);
        }
        else
        {
            obj.*member_ptr_ = std::any_cast<FieldType>(value);
        }
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class UppercaseStringFieldBinder
{
  public:
    using MemberPtr = std::string T::*;

    explicit UppercaseStringFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }

    void operator()(std::any& target, const std::any& value, const FormContext&) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();

        std::string bound_value = std::any_cast<std::string>(value);
        std::transform(bound_value.begin(), bound_value.end(), bound_value.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        obj.*member_ptr_ = std::move(bound_value);
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class DateFieldBinder
{
  public:
    using MemberPtr = std::optional<utils::TimePoint> T::*;
    explicit DateFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }
    void operator()(std::any& target, const std::any& value, const FormContext&) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();

        const auto& date_string = std::any_cast<const std::string&>(value);
        auto parsed = utils::TimestampParser::Parse(date_string);
        obj.*member_ptr_ = parsed;
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class StartDateFieldBinder
{
  public:
    using MemberPtr = std::optional<utils::TimePoint> T::*;
    explicit StartDateFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }

    void operator()(std::any& target, const std::any& value, const FormContext& context) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();
        const auto& date_string = std::any_cast<const std::string&>(value);

        auto timeframe = context.GetValue<dto::Timeframe>("timeframe");
        if (!timeframe.has_value())
        {
            obj.*member_ptr_ = utils::TimestampParser::Parse(date_string);
            return;
        }

        obj.*member_ptr_ = PeriodDateParser::ParsePeriodStart(*timeframe, date_string);
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class EndDateFieldBinder
{
  public:
    using MemberPtr = std::optional<utils::TimePoint> T::*;
    explicit EndDateFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }

    void operator()(std::any& target, const std::any& value, const FormContext& context) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();
        const auto& date_string = std::any_cast<const std::string&>(value);

        auto timeframe = context.GetValue<dto::Timeframe>("timeframe");
        if (!timeframe.has_value())
        {
            obj.*member_ptr_ = utils::TimestampParser::Parse(date_string);
            return;
        }

        obj.*member_ptr_ = PeriodDateParser::ParsePeriodEndExclusive(*timeframe, date_string);
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class TimeframeFieldBinder
{
  public:
    using MemberPtr = dto::Timeframe T::*;
    explicit TimeframeFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }
    void operator()(std::any& target, const std::any& value, const FormContext&) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();
        obj.*member_ptr_ = std::any_cast<dto::Timeframe>(value);
    }

  private:
    MemberPtr member_ptr_;
};

template <typename T> class OrderTypeFieldBinder
{
  public:
    using MemberPtr = dto::OrderType T::*;
    explicit OrderTypeFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr)
    {
    }
    void operator()(std::any& target, const std::any& value, const FormContext&) const
    {
        auto& obj = std::any_cast<std::reference_wrapper<T>>(target).get();
        obj.*member_ptr_ = std::any_cast<dto::OrderType>(value);
    }

  private:
    MemberPtr member_ptr_;
};

} // namespace form