#include "forms/candlestick/fields/currency_pair_field.hpp"

#include "dto/candlestick_query.hpp"
#include "forms/shared/binders/currency_pair_binder.hpp"
#include "forms/shared/validators/currency_pair_validator.hpp"

namespace candlestick
{

    CurrencyPairField::CurrencyPairField(std::set<std::string, std::less<>> allowed_currencies)
        : form::TextField("product_pair", "Currency pair",
                          forms::shared::CurrencyPairBinder<dto::CandlestickQuery>(),
                          std::make_shared<forms::shared::CurrencyPairValidator>(std::move(allowed_currencies))) {}

} // namespace candlestick
