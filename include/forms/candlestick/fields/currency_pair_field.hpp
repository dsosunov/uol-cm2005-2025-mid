#pragma once
#include <set>
#include <string>
#include "forms/shared/fields/product_pair_field.hpp"
#include "dto/candlestick_query.hpp"
namespace candlestick
{
  class CurrencyPairField : public forms::shared::CandlestickProductPairField
  {
  public:
    explicit CurrencyPairField(std::set<std::string, std::less<>> allowed_currencies);
  };
}