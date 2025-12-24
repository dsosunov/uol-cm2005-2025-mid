#include "actions/transaction/transaction_activity_summary_action.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "forms/transaction/activity_summary_form_data_provider.hpp"

#include <format>

TransactionActivitySummaryAction::TransactionActivitySummaryAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}

transaction_forms::ActivitySummaryForm TransactionActivitySummaryAction::CreateForm(
    ActionContext& context)
{
    auto form_data_provider =
        std::make_shared<transaction_forms::ActivitySummaryFormDataProvider>(trading_service_);
    return transaction_forms::ActivitySummaryForm(context.form_input_provider, context.output,
                                                  form_data_provider);
}

utils::ServiceResult<services::ActivityStats> TransactionActivitySummaryAction::ExecuteService(
    const dto::ActivitySummary& data, ActionContext& context)
{
    return transactions_service_->GetActivitySummary(data.timeframe, std::nullopt, std::nullopt);
}

void TransactionActivitySummaryAction::DisplayResults(
    const utils::ServiceResult<services::ActivityStats>& result, const dto::ActivitySummary& query,
    ActionContext& context)
{
    if (!result.success || !result.data.has_value())
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    const auto& stats = *result.data;
    DisplaySuccessHeader(context);

    DisplayField("Total Transactions", std::format("{}", stats.total_transactions), context);
    DisplayField("Total Volume", std::format("${:.2f}", stats.total_volume), context);
    DisplayField("Average Transaction Size", std::format("${:.2f}", stats.average_transaction_size),
                 context);
}
