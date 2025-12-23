#include "actions/transaction/transaction_show_by_pair_action.hpp"

#include <format>
#include <set>

#include "forms/transaction/product_pair_form.hpp"

static const std::set<std::string, std::less<>> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                                      "JPY", "AUD", "CHF", "CNY"};

void TransactionShowByPairAction::Execute(ActionContext &context)
{

  dto::TransactionQuery data;
  transaction_forms::ProductPairForm form(context.form_input_provider, context.output,
                                          kAllowedCurrencies);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void TransactionShowByPairAction::DisplayResults(const dto::TransactionQuery &data,
                                                 ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine(std::format("=== Transactions for {} ===", data.product_pair));
  context.output->WriteLine("1. Buy  - 100.00 @ 1.25 - 2025-12-22 10:30");
  context.output->WriteLine("2. Sell - 50.00  @ 1.26 - 2025-12-21 09:15");
  context.output->WriteLine("3. Buy  - 200.00 @ 1.24 - 2025-12-20 15:45");
}
