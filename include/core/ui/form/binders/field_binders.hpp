#pragma once
#include <any>
#include <functional>
#include <string>
#include <optional>
#include <stdexcept>

#include "core/ui/form/form_context.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

namespace form
{
  template <typename T, typename FieldType>
  class SimpleFieldBinder
  {
  public:
    using MemberPtr = FieldType T::*;

    explicit SimpleFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr) {}

    void operator()(std::any &target, const std::string &value, const FormContext &) const
    {
      auto &obj = std::any_cast<std::reference_wrapper<T>>(target).get();
      obj.*member_ptr_ = value;
    }

  private:
    MemberPtr member_ptr_;
  };

  template <typename T>
  class DateFieldBinder
  {
  public:
    using MemberPtr = std::optional<utils::TimePoint> T::*;

    explicit DateFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr) {}

    void operator()(std::any &target, const std::string &value, const FormContext &) const
    {
      auto &obj = std::any_cast<std::reference_wrapper<T>>(target).get();

      if (value.empty())
      {
        obj.*member_ptr_ = std::nullopt;
      }
      else
      {
        obj.*member_ptr_ = utils::ParseTimestamp(value);
      }
    }

  private:
    MemberPtr member_ptr_;
  };

  template <typename T>
  class TimeframeFieldBinder
  {
  public:
    using MemberPtr = dto::Timeframe T::*;

    explicit TimeframeFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr) {}

    void operator()(std::any &target, const std::string &value, const FormContext &) const
    {
      auto &obj = std::any_cast<std::reference_wrapper<T>>(target).get();

      if (value == "daily")
        obj.*member_ptr_ = dto::Timeframe::Daily;
      else if (value == "monthly")
        obj.*member_ptr_ = dto::Timeframe::Monthly;
      else if (value == "yearly")
        obj.*member_ptr_ = dto::Timeframe::Yearly;
      else
        throw std::invalid_argument("Invalid timeframe value: " + value);
    }

  private:
    MemberPtr member_ptr_;
  };

  template <typename T>
  class OrderTypeFieldBinder
  {
  public:
    using MemberPtr = dto::OrderType T::*;

    explicit OrderTypeFieldBinder(MemberPtr member_ptr) : member_ptr_(member_ptr) {}

    void operator()(std::any &target, const std::string &value, const FormContext &) const
    {
      auto &obj = std::any_cast<std::reference_wrapper<T>>(target).get();

      if (value == "asks")
        obj.*member_ptr_ = dto::OrderType::Asks;
      else if (value == "bids")
        obj.*member_ptr_ = dto::OrderType::Bids;
      else
        throw std::invalid_argument("Invalid order type value: " + value);
    }

  private:
    MemberPtr member_ptr_;
  };

}
