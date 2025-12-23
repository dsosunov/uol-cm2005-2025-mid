#include "actions/summary/candlestick_summary_action.hpp"

#include <set>

#include "forms/candlestick/candlestick_form.hpp"

static const std::set<std::string> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                         "JPY", "AUD", "CHF", "CNY"};

void CandlestickSummaryAction::Execute(ActionContext &context)
{
  dto::CandlestickQuery query;
  candlestick::CandlestickForm form(context.form_input_provider, context.output,
                                    kAllowedCurrencies);

  form::FormReadResult result = form.Read(query);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(query, context);
}

void CandlestickSummaryAction::DisplayResults(const dto::CandlestickQuery &query,
                                              ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Candlestick Summary Results ===");
  context.output->WriteLine("Product Pair: " + query.currency_base + "/" + query.currency_quote);
  context.output->WriteLine("Data Type: " + query.asks_bids);
  context.output->WriteLine("Timeframe: " + query.timeframe);

  if (!query.start_date.empty())
  {
    context.output->WriteLine("Start Date: " + query.start_date);
  }

  if (!query.end_date.empty())
  {
    context.output->WriteLine("End Date: " + query.end_date);
  }
}
