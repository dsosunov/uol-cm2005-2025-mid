#pragma once
#include <string>

namespace dto
{

struct TransactionQuery {
  std::string product_pair;
};

struct ActivitySummary {
  std::string timeframe;  // "daily", "monthly", or "yearly"
  std::string start_date;
  std::string end_date;
};

}
