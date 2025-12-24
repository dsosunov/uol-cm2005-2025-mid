#pragma once
#include "dto/candlestick_query.hpp"
#include "forms/shared/fields/product_pair_field.hpp"

#include <set>
#include <string>

namespace candlestick
{
class CurrencyPairField : public forms::shared::CandlestickProductPairField
{
  public:
    explicit CurrencyPairField(std::set<std::string, std::less<>> allowed_currencies);
};
} // namespace candlestick