#include "actions/transaction/transaction_activity_summary_action.hpp"

#include <format>

#include "forms/transaction/activity_summary_form.hpp"
#include "forms/transaction/activity_summary_form_data_provider.hpp"

TransactionActivitySummaryAction::TransactionActivitySummaryAction(
    std::shared_ptr<services::TransactionsService> transactions_service,
    std::shared_ptr<services::TradingService> trading_service)
    : transactions_service_(std::move(transactions_service)),
      trading_service_(std::move(trading_service))
{
}

void TransactionActivitySummaryAction::Execute(ActionContext &context)
{
  // Create form data provider that bridges service and form
  auto form_data_provider = std::make_shared<transaction_forms::ActivitySummaryFormDataProvider>(trading_service_);

  dto::ActivitySummary data;
  transaction_forms::ActivitySummaryForm form(context.form_input_provider, context.output,
                                              form_data_provider);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  // Get activity summary from service
  auto stats = transactions_service_->GetActivitySummary(data.timeframe, data.start_date, data.end_date);
  DisplayResults(data, stats, context);
}

void TransactionActivitySummaryAction::DisplayResults(const dto::ActivitySummary &query,
                                                      const services::ActivityStats &stats,
                                                      ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Activity Summary Results ===");
  context.output->WriteLine(std::format("Timeframe: {}", query.timeframe));
  context.output->WriteLine(std::format("Start Date: {}", query.start_date));
  context.output->WriteLine(std::format("End Date: {}", query.end_date));
  context.output->WriteLine("");
  context.output->WriteLine(std::format("Total Transactions: {}", stats.total_transactions));
  context.output->WriteLine(std::format("Total Volume: ${:.2f}", stats.total_volume));
  context.output->WriteLine(
      std::format("Average Transaction Size: ${:.2f}", stats.average_transaction_size));
}
