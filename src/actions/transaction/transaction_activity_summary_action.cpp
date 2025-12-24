#include "actions/transaction/transaction_activity_summary_action.hpp"

#include "core/actions/action_helper.hpp"
#include "core/utils/output_formatter.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "forms/transaction/activity_summary_form.hpp"
#include "forms/transaction/activity_summary_form_data_provider.hpp"

#include <format>

TransactionActivitySummaryAction::TransactionActivitySummaryAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}

void TransactionActivitySummaryAction::Execute(ActionContext& context)
{
    auto form_data_provider =
        std::make_shared<transaction_forms::ActivitySummaryFormDataProvider>(trading_service_);
    dto::ActivitySummary data;
    transaction_forms::ActivitySummaryForm form(context.form_input_provider, context.output,
                                                form_data_provider);
    if (auto form_result = form.Read(data);
        actions::ActionHelper::HandleFormCancellation(form_result, "Query", context))
    {
        return;
    }
    auto stats =
        transactions_service_->GetActivitySummary(data.timeframe, data.start_date, data.end_date);
    DisplayResults(data, stats, context);
}
void TransactionActivitySummaryAction::DisplayResults(const dto::ActivitySummary& query,
                                                      const services::ActivityStats& stats,
                                                      ActionContext& context) const
{
    context.output->WriteLine(utils::OutputFormatter::SectionHeader("Activity Summary Results"));
    context.output->WriteLine(
        std::format("Timeframe: {}", dto::TimeframeToString(query.timeframe)));
    if (query.start_date.has_value())
    {
        context.output->WriteLine(
            std::format("Start Date: {}", utils::FormatDate(*query.start_date)));
    }
    if (query.end_date.has_value())
    {
        context.output->WriteLine(std::format("End Date: {}", utils::FormatDate(*query.end_date)));
    }
    context.output->WriteLine("");
    context.output->WriteLine(std::format("Total Transactions: {}", stats.total_transactions));
    context.output->WriteLine(std::format("Total Volume: ${:.2f}", stats.total_volume));
    context.output->WriteLine(
        std::format("Average Transaction Size: ${:.2f}", stats.average_transaction_size));
}
