#include "actions/summary/candlestick_summary_action.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "forms/candlestick/candlestick_form_data_provider.hpp"

#include <format>

CandlestickSummaryAction::CandlestickSummaryAction(
    std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

candlestick::CandlestickForm CandlestickSummaryAction::CreateForm(ActionContext& context)
{
    auto form_data_provider =
        std::make_shared<candlestick::CandlestickFormDataProvider>(trading_service_);
    return candlestick::CandlestickForm(context.form_input_provider, context.output,
                                        form_data_provider);
}

utils::ServiceResult<services::CandlestickSummaryData> CandlestickSummaryAction::ExecuteService(
    const dto::CandlestickQuery& query, ActionContext& context)
{
    return trading_service_->GetCandlestickSummary(query.currency_base, query.currency_quote,
                                                   query.order_type, query.timeframe,
                                                   query.start_date, query.end_date);
}

void CandlestickSummaryAction::DisplayResults(
    const utils::ServiceResult<services::CandlestickSummaryData>& result,
    const dto::CandlestickQuery& query, ActionContext& context)
{
    if (!result.success)
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    if (!result.data.has_value() || result.data->periods.empty())
    {
        DisplayFailureHeader("No data available", context);
        return;
    }

    DisplaySuccessHeader(context);

    DisplayField("Product Pair", std::format("{}/{}", query.currency_base, query.currency_quote),
                 context);
    DisplayField("Data Type", dto::OrderTypeToString(query.order_type), context);
    DisplayField("Timeframe", dto::TimeframeToString(query.timeframe), context);

    if (query.start_date.has_value())
    {
        DisplayField("Start Date", utils::FormatDate(*query.start_date), context);
    }

    if (query.end_date.has_value())
    {
        DisplayField("End Date", utils::FormatDate(*query.end_date), context);
    }

    WriteEmptyLine(context);

    WriteLine(std::format("Analysis per {}:", dto::TimeframeToString(result.data->timeframe)),
              context);

    WriteLine(
        std::format("{:<12} {:<15} {:<15} {:<15} {:<15}", "Period", "Open", "High", "Low", "Close"),
        context);
    WriteLine(std::string(12 + 15 + 15 + 15 + 15 + 4, '-'), context);

    for (const auto& period : result.data->periods)
    {
        WriteLine(std::format("{:<12} {:<15.4f} {:<15.4f} {:<15.4f} {:<15.4f}", period.period,
                              period.open, period.high, period.low, period.close),
                  context);
    }

    WriteEmptyLine(context);
    WriteLine(std::format("Total periods analyzed: {}", result.data->periods.size()), context);
}
