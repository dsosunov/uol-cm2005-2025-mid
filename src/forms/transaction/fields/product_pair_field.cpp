#include "forms/transaction/fields/product_pair_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"
#include "forms/shared/validators/currency_pair_validator.hpp"

namespace transaction_forms
{

    ProductPairField::ProductPairField(std::set<std::string> allowed_currencies)
        : form::TextField("product_pair", "Product pair",
                          form::SimpleFieldBinder<dto::TransactionQuery, std::string>(
                              &dto::TransactionQuery::product_pair),
                          std::make_shared<forms::shared::CurrencyPairValidator>(std::move(allowed_currencies))) {}

} // namespace transaction_forms
