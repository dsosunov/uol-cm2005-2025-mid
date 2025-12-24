#pragma once
#include "core/ui/form/form_context.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

#include <any>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>

namespace form
{
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
        auto parsed = utils::ParseTimestamp(date_string);
        obj.*member_ptr_ = parsed;
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