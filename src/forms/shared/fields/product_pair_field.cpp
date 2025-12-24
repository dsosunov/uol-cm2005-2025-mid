#include "forms/shared/fields/product_pair_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/candlestick_query.hpp"
#include "dto/transaction_query.hpp"
#include "forms/shared/binders/currency_pair_binder.hpp"
#include "forms/shared/validators/currency_pair_validator.hpp"

namespace forms::shared
{
CandlestickProductPairField::CandlestickProductPairField(
    std::set<std::string, std::less<>> allowed_currencies)
    : form::TextField("product_pair", "Currency pair", CurrencyPairBinder<dto::CandlestickQuery>(),
                      std::make_shared<CurrencyPairValidator>(std::move(allowed_currencies)))
{
}
template <typename TDTO>
ProductPairField<TDTO>::ProductPairField(std::set<std::string, std::less<>> allowed_currencies,
                                         MemberPtr member_ptr)
    : form::TextField("product_pair", "Product pair",
                      form::SimpleFieldBinder<TDTO, std::string>(member_ptr),
                      std::make_shared<CurrencyPairValidator>(std::move(allowed_currencies)))
{
}
template class ProductPairField<dto::TransactionQuery>;
} // namespace forms::shared