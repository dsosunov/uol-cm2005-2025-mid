#include "forms/transaction/fields/product_pair_field.hpp"

#include "dto/transaction_query.hpp"
#include "forms/shared/fields/product_pair_field.hpp"

namespace transaction_forms
{
ProductPairField::ProductPairField(std::set<std::string, std::less<>> allowed_currencies)
    : forms::shared::ProductPairField<dto::TransactionQuery>(std::move(allowed_currencies),
                                                             &dto::TransactionQuery::product_pair)
{
}
} // namespace transaction_forms