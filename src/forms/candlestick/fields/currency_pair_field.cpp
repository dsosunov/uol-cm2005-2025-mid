#include "forms/candlestick/fields/currency_pair_field.hpp"

#include "forms/shared/fields/product_pair_field.hpp"

namespace candlestick
{

CurrencyPairField::CurrencyPairField(std::set<std::string, std::less<>> allowed_currencies)
    : forms::shared::CandlestickProductPairField(std::move(allowed_currencies))
{
}

} // namespace candlestick