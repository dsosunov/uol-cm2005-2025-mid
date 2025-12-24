#pragma once
#include "dto/transaction_query.hpp"
#include "forms/shared/fields/currency_field.hpp"

#include <set>
#include <string>

namespace transaction_forms
{
class CurrencyField : public forms::shared::CurrencyField<dto::TransactionQuery>
{
  public:
    explicit CurrencyField(std::set<std::string, std::less<>> allowed_currencies);
};
} // namespace transaction_forms
