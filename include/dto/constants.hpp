#pragma once
#include <string>
#include <string_view>

namespace dto
{

  enum class Timeframe
  {
    Daily,
    Monthly,
    Yearly
  };

  enum class OrderType
  {
    Asks,
    Bids
  };

  inline std::string_view TimeframeToString(Timeframe timeframe)
  {
    switch (timeframe)
    {
    case Timeframe::Daily:
      return "daily";
    case Timeframe::Monthly:
      return "monthly";
    case Timeframe::Yearly:
      return "yearly";
    default:
      return "";
    }
  }

  inline std::string_view OrderTypeToString(OrderType order_type)
  {
    switch (order_type)
    {
    case OrderType::Asks:
      return "asks";
    case OrderType::Bids:
      return "bids";
    default:
      return "";
    }
  }

}
