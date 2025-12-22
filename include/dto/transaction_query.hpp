#pragma once
#include <string>

namespace dto {

struct TransactionQuery {
  std::string product_pair;
};

struct ActivitySummary {
  std::string timeframe;  // Use dto::timeframe constants
  std::string start_date;
  std::string end_date;
};

}  // namespace dto
