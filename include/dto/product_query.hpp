#pragma once
#include <string>

namespace dto
{

struct ProductQuery {
  std::string currency_base;
  std::string currency_quote;
  std::string query_type;
  std::string timeframe;
  std::string start_date;
  std::string end_date;
};

}
