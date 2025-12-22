#pragma once
#include <string>

namespace dto
{

struct CandlestickQuery {
  std::string currency_base;
  std::string currency_quote;
  std::string asks_bids;  // "asks" or "bids"
  std::string timeframe;  // "daily", "monthly", or "yearly"
  std::string start_date;
  std::string end_date;
};

}
