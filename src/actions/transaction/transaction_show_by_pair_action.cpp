#include "actions/transaction/transaction_show_by_pair_action.hpp"

#include "core/utils/time_utils.hpp"

#include <format>

TransactionShowByPairAction::TransactionShowByPairAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}

transaction_forms::ProductPairForm TransactionShowByPairAction::CreateForm(ActionContext& context)
{
    auto result = trading_service_->GetAvailableProducts();
    std::set<std::string, std::less<>> allowed_currencies;
    if (result.success && result.data.has_value())
    {
        allowed_currencies = *result.data;
    }
    return transaction_forms::ProductPairForm(context.form_input_provider, context.output,
                                              allowed_currencies);
}

utils::ServiceResult<std::vector<services::Transaction>> TransactionShowByPairAction::
    ExecuteService(const dto::TransactionQuery& data, ActionContext& context)
{
    return transactions_service_->GetTransactionsByPair(data.product_pair);
}

void TransactionShowByPairAction::DisplayResults(
    const utils::ServiceResult<std::vector<services::Transaction>>& result,
    const dto::TransactionQuery& data, ActionContext& context)
{
    if (!result.success || !result.data.has_value())
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    const auto& transactions = *result.data;
    DisplaySuccessHeader(context);
    DisplayField("Product Pair", data.product_pair, context);
    WriteEmptyLine(context);

    if (transactions.empty())
    {
        WriteLine("No transactions found for this product pair.", context);
    }
    else
    {
        int index = 1;
        for (const auto& transaction : transactions)
        {
            WriteLine(std::format("{}. {} - {:.2f} @ {:.4f} - {}", index, transaction.type,
                                  transaction.amount, transaction.price,
                                  utils::FormatTimestamp(transaction.timestamp)),
                      context);
            ++index;
        }
    }
}
