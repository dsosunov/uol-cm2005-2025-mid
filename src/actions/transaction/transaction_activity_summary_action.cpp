#include "actions/transaction/transaction_activity_summary_action.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "forms/transaction/activity_summary_form_data_provider.hpp"

#include <format>

TransactionActivitySummaryAction::TransactionActivitySummaryAction(
    std::shared_ptr<services::TransactionsService> transactions_service)
    : transactions_service_(std::move(transactions_service))
{
}

transaction_forms::ActivitySummaryForm TransactionActivitySummaryAction::CreateForm(
    ActionContext& context)
{
    int user_id = 0;
    if (auto au = context.auth_service->GetAuthenticatedUser(); au.success)
    {
        user_id = au.data->id;
    }
    else
    {
        WriteLine("Please log in first to view transactions", context);
    }

    auto form_data_provider = std::make_shared<transaction_forms::ActivitySummaryFormDataProvider>(
        transactions_service_, user_id);
    return transaction_forms::ActivitySummaryForm(context.form_input_provider, context.output,
                                                  form_data_provider);
}

utils::ServiceResult<services::ActivityStats> TransactionActivitySummaryAction::ExecuteService(
    const dto::ActivitySummary& data, ActionContext& context)
{
    if (auto au = context.auth_service->GetAuthenticatedUser(); au.success)
    {
        return transactions_service_->GetActivitySummary(au.data->id, data.timeframe,
                                                         data.start_date, data.end_date);
    }

    return utils::ServiceResult<services::ActivityStats>::Failure(
        "Please log in first to view transactions");
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

    DisplayField("Timeframe", dto::TimeframeToString(query.timeframe), context);

    if (query.start_date.has_value() && query.end_date.has_value())
    {
        DisplayField("Span",
                     std::format("{} - {}", utils::FormatDate(*query.start_date),
                                 utils::FormatDate(*query.end_date)),
                     context);
    }
    else if (query.start_date.has_value())
    {
        DisplayField("Start Date", utils::FormatDate(*query.start_date), context);
    }
    else if (query.end_date.has_value())
    {
        DisplayField("End Date", utils::FormatDate(*query.end_date), context);
    }

    WriteEmptyLine(context);

    std::string period_header = "Period";
    if (query.timeframe == dto::Timeframe::Daily)
    {
        period_header = "Day";
    }
    else if (query.timeframe == dto::Timeframe::Monthly)
    {
        period_header = "Month";
    }
    else if (query.timeframe == dto::Timeframe::Yearly)
    {
        period_header = "Year";
    }

    WriteLine(std::format("{:<12} {:>12} {:>18} {:>18}", period_header, "Txns", "Total Volume",
                          "Avg Size"),
              context);
    WriteLine(std::string(12 + 1 + 12 + 1 + 18 + 1 + 18, '-'), context);

    if (stats.per_period.empty())
    {
        WriteLine("No transactions found for selected timeframe.", context);
    }
    else
    {
        for (const auto& row : stats.per_period)
        {
            WriteLine(std::format("{:<12} {:>12} {:>18} {:>18}", row.period,
                                  std::format("{}", row.total_transactions),
                                  std::format("${:.4f}", row.total_volume),
                                  std::format("${:.4f}", row.average_transaction_size)),
                      context);
        }
    }

    WriteEmptyLine(context);

    WriteLine(std::format("{:<28} {}", "Metric", "Value"), context);
    WriteLine(std::string(28 + 1 + 24, '-'), context);
    WriteLine(
        std::format("{:<28} {}", "Total Transactions", std::format("{}", stats.total_transactions)),
        context);
    WriteLine(std::format("{:<28} {}", "Total Volume", std::format("${:.4f}", stats.total_volume)),
              context);
    WriteLine(std::format("{:<28} {}", "Average Transaction Size",
                          std::format("${:.4f}", stats.average_transaction_size)),
              context);
}
