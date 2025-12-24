#include "actions/transaction/transaction_show_by_pair_action.hpp"

#include "core/actions/action_helper.hpp"
#include "core/utils/output_formatter.hpp"
#include "core/utils/time_utils.hpp"
#include "forms/transaction/product_pair_form.hpp"

#include <format>

TransactionShowByPairAction::TransactionShowByPairAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}
void TransactionShowByPairAction::Execute(ActionContext& context)
{
    auto allowed_currencies = trading_service_->GetAvailableProducts();
    dto::TransactionQuery data;
    transaction_forms::ProductPairForm form(context.form_input_provider, context.output,
                                            allowed_currencies);
    if (auto form_result = form.Read(data);
        actions::ActionHelper::HandleFormCancellation(form_result, "Query", context))
    {
        return;
    }
    auto transactions = transactions_service_->GetTransactionsByPair(data.product_pair);
    DisplayResults(data.product_pair, transactions, context);
}
void TransactionShowByPairAction::DisplayResults(
    const std::string& product_pair, const std::vector<services::Transaction>& transactions,
    ActionContext& context) const
{
    context.output->WriteLine(
        utils::OutputFormatter::SectionHeader(std::format("Transactions for {}", product_pair)));
    if (transactions.empty())
    {
        context.output->WriteLine("No transactions found for this product pair.");
    }
    else
    {
        int index = 1;
        for (const auto& transaction : transactions)
        {
            context.output->WriteLine(std::format(
                "{}. {} - {:.2f} @ {:.4f} - {}", index, transaction.type, transaction.amount,
                transaction.price, utils::FormatTimestamp(transaction.timestamp)));
            ++index;
        }
    }
}
