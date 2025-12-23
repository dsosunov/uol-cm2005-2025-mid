#include "actions/transaction/transaction_show_last5_action.hpp"

#include <format>

TransactionShowLast5Action::TransactionShowLast5Action(
    std::shared_ptr<services::TransactionsService> transactions_service)
    : transactions_service_(std::move(transactions_service))
{
}

void TransactionShowLast5Action::Execute(ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Last 5 Transactions ===");

  // Get last 5 transactions from service
  auto transactions = transactions_service_->GetLastTransactions(5);

  if (transactions.empty())
  {
    context.output->WriteLine("No transactions found.");
  }
  else
  {
    int index = 1;
    for (const auto &transaction : transactions)
    {
      context.output->WriteLine(
          std::format("{}. {} - {} - {:.2f} @ {:.4f} - {}", index++, transaction.product_pair,
                      transaction.type, transaction.amount, transaction.price,
                      transaction.timestamp));
    }
  }
}
