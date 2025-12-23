#include "actions/transaction/transaction_activity_summary_action.hpp"

#include <format>

#include "forms/transaction/activity_summary_form.hpp"

void TransactionActivitySummaryAction::Execute(ActionContext &context)
{
  dto::ActivitySummary data;
  transaction_forms::ActivitySummaryForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void TransactionActivitySummaryAction::DisplayResults(const dto::ActivitySummary &data,
                                                      ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Activity Summary Results ===");
  context.output->WriteLine(std::format("Timeframe: {}", data.timeframe));
  context.output->WriteLine(std::format("Start Date: {}", data.start_date));
  context.output->WriteLine(std::format("End Date: {}", data.end_date));
  context.output->WriteLine("");
  context.output->WriteLine("Total Transactions: 45");
  context.output->WriteLine("Total Volume: $125,000.00");
  context.output->WriteLine("Average Transaction Size: $2,777.78");
}
