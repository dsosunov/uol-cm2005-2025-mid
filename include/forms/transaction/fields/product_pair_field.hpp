#pragma once
#include <set>
#include <string>
#include "dto/transaction_query.hpp"
#include "forms/shared/fields/product_pair_field.hpp"
namespace transaction_forms
{
  class ProductPairField : public forms::shared::ProductPairField<dto::TransactionQuery>
  {
  public:
    explicit ProductPairField(std::set<std::string, std::less<>> allowed_currencies);
  };
}