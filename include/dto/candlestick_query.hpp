#pragma once
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include <string>
#include <optional>

namespace dto
{

  struct CandlestickQuery
  {
    std::string currency_base;
    std::string currency_quote;
    OrderType order_type;
    Timeframe timeframe;
    std::optional<utils::TimePoint> start_date;
    std::optional<utils::TimePoint> end_date;
  };

}
