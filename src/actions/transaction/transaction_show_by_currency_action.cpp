#include "actions/transaction/transaction_show_by_currency_action.hpp"

#include "core/utils/time_utils.hpp"

#include <format>

TransactionShowByCurrencyAction::TransactionShowByCurrencyAction(
    std::shared_ptr<services::TransactionsService> transactions_service)
    : transactions_service_(std::move(transactions_service))
{
}

transaction_forms::CurrencyForm TransactionShowByCurrencyAction::CreateForm(ActionContext& context)
{
    auto result = transactions_service_->GetAvailableCurrencies();
    std::set<std::string, std::less<>> allowed_currencies;
    if (result.success && result.data.has_value())
    {
        allowed_currencies = *result.data;
    }
    return transaction_forms::CurrencyForm(context.form_input_provider, context.output,
                                           allowed_currencies);
}

utils::ServiceResult<std::vector<services::WalletTransaction>> TransactionShowByCurrencyAction::
    ExecuteService(const dto::TransactionQuery& data, ActionContext& context)
{
    if (!context.auth_service->IsAuthenticated())
    {
        return utils::ServiceResult<std::vector<services::WalletTransaction>>::Failure(
            "Please log in first to view transactions");
    }

    return transactions_service_->GetTransactionsByCurrency(data.currency);
}

void TransactionShowByCurrencyAction::DisplayResults(
    const utils::ServiceResult<std::vector<services::WalletTransaction>>& result,
    const dto::TransactionQuery& data, ActionContext& context)
{
    if (!result.success || !result.data.has_value())
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    const auto& transactions = *result.data;
    DisplaySuccessHeader(context);

    DisplayField("Currency", data.currency, context);
    WriteEmptyLine(context);

    if (transactions.empty())
    {
        WriteLine("No transactions found for this currency.", context);
    }
    else
    {
        int index = 1;
        for (const auto& transaction : transactions)
        {
            WriteLine(std::format("{}. {} - {:.2f} - {}", index, transaction.type,
                                  transaction.amount,
                                  utils::FormatTimestamp(transaction.timestamp)),
                      context);
            ++index;
        }
    }
}
