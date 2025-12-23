#include "actions/summary/candlestick_summary_action.hpp"

#include <format>

#include "forms/candlestick/candlestick_form.hpp"
#include "forms/candlestick/candlestick_form_data_provider.hpp"

CandlestickSummaryAction::CandlestickSummaryAction(
    std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

void CandlestickSummaryAction::Execute(ActionContext &context)
{
  // Create form data provider that bridges service and form
  auto form_data_provider = std::make_shared<candlestick::CandlestickFormDataProvider>(trading_service_);

  dto::CandlestickQuery query;
  candlestick::CandlestickForm form(context.form_input_provider, context.output, form_data_provider);

  if (auto result = form.Read(query); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  // Get candlestick data from service
  auto result = trading_service_->GetCandlestickData(query.currency_base, query.currency_quote,
                                                     query.asks_bids, query.timeframe,
                                                     query.start_date, query.end_date);
  DisplayResults(query, result, context);
}

void CandlestickSummaryAction::DisplayResults(const dto::CandlestickQuery &query,
                                              const services::CandlestickResult &result,
                                              ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Candlestick Summary Results ===");
  context.output->WriteLine(
      std::format("Product Pair: {}/{}", query.currency_base, query.currency_quote));
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

  if (result.success && !result.data.empty())
  {
    context.output->WriteLine("");
    context.output->WriteLine("Sample Data Points:");
    int count = 0;
    for (const auto &candle : result.data)
    {
      if (count++ >= 3)
        break; // Show first 3 for brevity
      context.output->WriteLine(std::format("  {} - O:{:.4f} H:{:.4f} L:{:.4f} C:{:.4f} V:{:.0f}",
                                            candle.timestamp, candle.open, candle.high, candle.low,
                                            candle.close, candle.volume));
    }
    context.output->WriteLine(std::format("  ... ({} total data points)", result.data.size()));
  }
}
