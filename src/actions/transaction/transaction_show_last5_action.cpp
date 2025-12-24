#include "actions/transaction/transaction_show_last5_action.hpp"

#include "core/utils/output_formatter.hpp"
#include "core/utils/time_utils.hpp"

#include <format>

TransactionShowLast5Action::TransactionShowLast5Action(
    std::shared_ptr<services::TransactionsService> transactions_service)
    : transactions_service_(std::move(transactions_service))
{
}
void TransactionShowLast5Action::Execute(ActionContext& context)
{
    context.output->WriteLine(utils::OutputFormatter::SectionHeader("Last 5 Transactions"));
    auto transactions = transactions_service_->GetLastTransactions(5);
    if (transactions.empty())
    {
        context.output->WriteLine("No transactions found.");
    }
    else
    {
        int index = 1;
        for (const auto& transaction : transactions)
        {
            context.output->WriteLine(std::format("{}. {} - {} - {:.2f} @ {:.4f} - {}", index,
                                                  transaction.product_pair, transaction.type,
                                                  transaction.amount, transaction.price,
                                                  utils::FormatTimestamp(transaction.timestamp)));
            ++index;
        }
    }
}
