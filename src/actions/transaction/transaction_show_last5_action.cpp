#include "actions/transaction/transaction_show_last5_action.hpp"

#include "core/utils/time_utils.hpp"

#include <algorithm>
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
    if (!context.auth_service->IsAuthenticated())
    {
        return utils::ServiceResult<std::vector<services::WalletTransaction>>::Failure(
            "Please log in first to view transactions");
    }

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

    auto transactions = *result.data;
    DisplaySuccessHeader(context);

    if (transactions.empty())
    {
        WriteLine("No transactions found.", context);
    }
    else
    {
        std::reverse(transactions.begin(), transactions.end());

        WriteLine(std::format("{:<4} {:<12} {:<12} {:<12} {:<20}", "#", "Currency", "Type",
                              "Amount", "Timestamp"),
                  context);
        WriteLine(std::string(4 + 12 + 12 + 12 + 20 + 4, '-'), context);

        int index = 1;
        for (const auto& transaction : transactions)
        {
            WriteLine(std::format("{:<4} {:<12} {:<12} {:<12.4f} {:<20}", index,
                                  transaction.currency, transaction.type, transaction.amount,
                                  utils::FormatTimestamp(transaction.timestamp)),
                      context);
            ++index;
        }
    }
}
