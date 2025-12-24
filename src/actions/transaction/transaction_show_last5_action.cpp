#include "actions/transaction/transaction_show_last5_action.hpp"

#include "core/utils/time_utils.hpp"

#include <format>

TransactionShowLast5Action::TransactionShowLast5Action(
    std::shared_ptr<services::TransactionsService> transactions_service)
    : transactions_service_(std::move(transactions_service))
{
}

shared_forms::EmptyForm TransactionShowLast5Action::CreateForm(ActionContext& context)
{
    return shared_forms::EmptyForm();
}

utils::ServiceResult<std::vector<services::WalletTransaction>> TransactionShowLast5Action::
    ExecuteService([[maybe_unused]] const EmptyRequest& data, ActionContext& context)
{
    return transactions_service_->GetLastTransactions(5);
}

void TransactionShowLast5Action::DisplayResults(
    const utils::ServiceResult<std::vector<services::WalletTransaction>>& result,
    const EmptyRequest& data, ActionContext& context)
{
    if (!result.success || !result.data.has_value())
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    const auto& transactions = *result.data;
    DisplaySuccessHeader(context);

    if (transactions.empty())
    {
        WriteLine("No transactions found.", context);
    }
    else
    {
        int index = 1;
        for (const auto& transaction : transactions)
        {
            WriteLine(std::format("{}. {} - {} - {:.2f} - {}", index, transaction.currency,
                                  transaction.type, transaction.amount,
                                  utils::FormatTimestamp(transaction.timestamp)),
                      context);
            ++index;
        }
    }
}
