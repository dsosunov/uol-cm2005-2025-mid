#include "actions/transaction_activity_summary_action.hpp"

#include "forms/transaction/activity_summary_form.hpp"

void TransactionActivitySummaryAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== Activity Summary ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::ActivitySummary data;
  transaction_forms::ActivitySummaryForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled) {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void TransactionActivitySummaryAction::DisplayResults(const dto::ActivitySummary& data,
                                                      ActionContext& context) {
  context.output->WriteLine("");
  context.output->WriteLine("=== Activity Summary Results ===");
  context.output->WriteLine("Timeframe: " + data.timeframe);
  context.output->WriteLine("Start Date: " + data.start_date);
  context.output->WriteLine("End Date: " + data.end_date);
  context.output->WriteLine("");
  context.output->WriteLine("Total Transactions: 45");
  context.output->WriteLine("Total Volume: $125,000.00");
  context.output->WriteLine("Average Transaction Size: $2,777.78");
}
