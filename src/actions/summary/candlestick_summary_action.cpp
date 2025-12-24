#include "actions/summary/candlestick_summary_action.hpp"

#include "core/actions/action_helper.hpp"
#include "core/utils/output_formatter.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "forms/candlestick/candlestick_form.hpp"
#include "forms/candlestick/candlestick_form_data_provider.hpp"

#include <format>

CandlestickSummaryAction::CandlestickSummaryAction(
    std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

void CandlestickSummaryAction::Execute(ActionContext& context)
{
    auto form_data_provider =
        std::make_shared<candlestick::CandlestickFormDataProvider>(trading_service_);

    dto::CandlestickQuery query;
    candlestick::CandlestickForm form(context.form_input_provider, context.output,
                                      form_data_provider);

    if (auto form_result = form.Read(query);
        actions::ActionHelper::HandleFormCancellation(form_result, "Query", context))
    {
        return;
    }

    auto result = trading_service_->GetCandlestickSummary(query.currency_base, query.currency_quote,
                                                          query.order_type, query.timeframe,
                                                          query.start_date, query.end_date);
    DisplayResults(query, result, context);
}

void CandlestickSummaryAction::DisplayResults(
    const dto::CandlestickQuery& query,
    const utils::ServiceResult<services::CandlestickSummaryData>& result,
    ActionContext& context) const
{
    context.output->WriteLine(utils::OutputFormatter::SectionHeader("Candlestick Summary Results"));
    context.output->WriteLine(
        std::format("Product Pair: {}/{}", query.currency_base, query.currency_quote));
    context.output->WriteLine(
        std::format("Data Type: {}", dto::OrderTypeToString(query.order_type)));
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

    if (!result.success || result.data->periods.empty())
    {
        context.output->WriteLine("");
        context.output->WriteLine(result.message);
        return;
    }

    context.output->WriteLine("");
    context.output->WriteLine(
        std::format("Analysis per {}:", dto::TimeframeToString(result.data->timeframe)));
    context.output->WriteLine(std::format("{:<12} {:<15} {:<15} {:<15} {:<10}", "Period",
                                          "Min Price", "Max Price", "Avg Volume", "Trades"));
    context.output->WriteLine(std::string(12 + 15 + 15 + 15 + 10 + 4, '-'));

    for (const auto& period : result.data->periods)
    {
        context.output->WriteLine(std::format("{:<12} {:<15.4f} {:<15.4f} {:<15.2f} {:<10}",
                                              period.period, period.min_price, period.max_price,
                                              period.avg_volume, period.trade_count));
    }

    context.output->WriteLine("");
    context.output->WriteLine(
        std::format("Total periods analyzed: {}", result.data->periods.size()));
}
