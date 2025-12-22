#pragma once
#include <string>

namespace dto {

struct CandlestickQuery {
  std::string currency_base;
  std::string currency_quote;
  std::string asks_bids;  // Use dto::order_type constants
  std::string timeframe;  // Use dto::timeframe constants
  std::string start_date;
  std::string end_date;
};

}  // namespace dto
