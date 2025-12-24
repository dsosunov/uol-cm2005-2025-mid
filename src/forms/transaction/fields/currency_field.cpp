#include "forms/transaction/fields/currency_field.hpp"

#include "dto/transaction_query.hpp"
#include "forms/shared/fields/currency_field.hpp"

namespace transaction_forms
{

CurrencyField::CurrencyField(std::set<std::string, std::less<>> allowed_currencies)
    : forms::shared::CurrencyField<dto::TransactionQuery>(std::move(allowed_currencies),
                                                          &dto::TransactionQuery::currency)
{
}

} // namespace transaction_forms
