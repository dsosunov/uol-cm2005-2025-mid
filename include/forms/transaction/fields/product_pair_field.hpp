#pragma once
#include "dto/transaction_query.hpp"
#include "forms/shared/fields/product_pair_field.hpp"

#include <set>
#include <string>

namespace transaction_forms
{
class ProductPairField : public forms::shared::ProductPairField<dto::TransactionQuery>
{
  public:
    explicit ProductPairField(std::set<std::string, std::less<>> allowed_currencies);
};
} // namespace transaction_forms