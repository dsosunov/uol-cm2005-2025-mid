#include "actions/summary/candlestick_summary_action.hpp"

#include <format>
#include <set>

#include "forms/candlestick/candlestick_form.hpp"

static const std::set<std::string, std::less<>> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                                      "JPY", "AUD", "CHF", "CNY"};

void CandlestickSummaryAction::Execute(ActionContext &context)
{
  dto::CandlestickQuery query;
  candlestick::CandlestickForm form(context.form_input_provider, context.output,
                                    kAllowedCurrencies);

  if (auto result = form.Read(query); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(query, context);
}

void CandlestickSummaryAction::DisplayResults(const dto::CandlestickQuery &query,
                                              ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Candlestick Summary Results ===");
  context.output->WriteLine(std::format("Product Pair: {}/{}", query.currency_base, query.currency_quote));
  context.output->WriteLine(std::format("Data Type: {}", query.asks_bids));
  context.output->WriteLine(std::format("Timeframe: {}", query.timeframe));

  if (!query.start_date.empty())
  {
    context.output->WriteLine(std::format("Start Date: {}", query.start_date));
  }

  if (!query.end_date.empty())
  {
    context.output->WriteLine(std::format("End Date: {}", query.end_date));
  }
}
