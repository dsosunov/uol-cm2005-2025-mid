#include "forms/candlestick/fields/currency_pair_field.hpp"

#include "dto/candlestick_query.hpp"
#include "forms/shared/binders/currency_pair_binder.hpp"
#include "forms/shared/validators/currency_pair_validator.hpp"

namespace candlestick {

CurrencyPairField::CurrencyPairField(std::set<std::string> allowed_currencies)
    : form::TextField("product_pair", "Enter product pair (e.g., USD/CAD)",
                      forms::shared::CurrencyPairBinder<dto::CandlestickQuery>(),
                      forms::shared::CurrencyPairValidator(std::move(allowed_currencies))) {}

}  // namespace candlestick
