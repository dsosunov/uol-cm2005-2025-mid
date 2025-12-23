#include "actions/transaction/transaction_show_by_pair_action.hpp"

#include <format>

#include "core/utils/time_utils.hpp"
#include "forms/transaction/product_pair_form.hpp"

TransactionShowByPairAction::TransactionShowByPairAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}

void TransactionShowByPairAction::Execute(ActionContext &context)
{
  auto allowed_currencies = trading_service_->GetAvailableProducts();

  dto::TransactionQuery data;
  transaction_forms::ProductPairForm form(context.form_input_provider, context.output,
                                          allowed_currencies);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }
  auto transactions = transactions_service_->GetTransactionsByPair(data.product_pair);
  DisplayResults(data.product_pair, transactions, context);
}

void TransactionShowByPairAction::DisplayResults(
    const std::string &product_pair, const std::vector<services::Transaction> &transactions,
    ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine(std::format("=== Transactions for {} ===", product_pair));

  if (transactions.empty())
  {
    context.output->WriteLine("No transactions found for this product pair.");
  }
  else
  {
    int index = 1;
    for (const auto &transaction : transactions)
    {
      context.output->WriteLine(
          std::format("{}. {} - {:.2f} @ {:.4f} - {}", index, transaction.type,
                      transaction.amount, transaction.price, utils::FormatTimestamp(transaction.timestamp)));
      ++index;
    }
  }
}
