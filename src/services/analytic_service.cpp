#include "services/analytic_service.hpp"

#include "services/trading_service.hpp"

#include <algorithm>
#include <map>
#include <utility>

namespace services
{
AnalyticService::AnalyticService(std::shared_ptr<TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

void AnalyticService::ProcessSummaryOrder(
    const OrderRecord& order, std::string_view product_pair, const utils::DateFilter& date_filter,
    dto::Timeframe timeframe,
    std::map<std::string, AnalyticService::PeriodStats, std::less<>>& aggregated_data) const
{
    if (order.product_pair != product_pair)
    {
        return;
    }

    if (!date_filter.IsInRange(order.timestamp))
    {
        return;
    }

    std::string formatted_date = utils::FormatDate(order.timestamp);
    std::string period_key;

    if (timeframe == dto::Timeframe::Monthly && formatted_date.length() >= 7)
    {
        period_key = formatted_date.substr(0, 7);
    }
    else if (timeframe == dto::Timeframe::Yearly && formatted_date.length() >= 4)
    {
        period_key = formatted_date.substr(0, 4);
    }
    else
    {
        period_key = formatted_date;
    }

    auto& stats = aggregated_data[period_key];

    if (stats.count == 0)
    {
        stats.first_timestamp = order.timestamp;
        stats.last_timestamp = order.timestamp;
        stats.open = order.price;
        stats.high = order.price;
        stats.low = order.price;
        stats.close = order.price;
    }
    else
    {
        stats.high = std::max(stats.high, order.price);
        stats.low = std::min(stats.low, order.price);

        if (order.timestamp < stats.first_timestamp)
        {
            stats.first_timestamp = order.timestamp;
            stats.open = order.price;
        }

        if (order.timestamp > stats.last_timestamp)
        {
            stats.last_timestamp = order.timestamp;
            stats.close = order.price;
        }
    }

    stats.total_volume += order.amount;
    stats.count++;
}

utils::ServiceResult<CandlestickSummaryData> AnalyticService::GetCandlestickSummary(
    std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
    dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
    const std::optional<utils::TimePoint>& end_date) const
{
    if (!trading_service_)
    {
        return utils::ServiceResult<CandlestickSummaryData>::Failure(
            "Trading service is not configured");
    }

    std::string product_pair = std::string(currency_base) + "/" + std::string(currency_quote);
    std::map<std::string, PeriodStats, std::less<>> aggregated_data;
    utils::DateFilter date_filter = utils::DateFilter::Create(start_date, end_date);

    if (auto read_result = trading_service_->ForEachOrder(
            order_type,
            [this, &product_pair, &date_filter, &timeframe,
             &aggregated_data](const OrderRecord& order) {
                ProcessSummaryOrder(order, product_pair, date_filter, timeframe, aggregated_data);
            });
        !read_result.success)
    {
        return utils::ServiceResult<CandlestickSummaryData>::Failure(read_result.message);
    }

    if (aggregated_data.empty())
    {
        return utils::ServiceResult<CandlestickSummaryData>::Failure(
            "No data found for the specified currency pair and date range");
    }

    std::vector<Candlestick> candlesticks;
    candlesticks.reserve(aggregated_data.size());
    for (const auto& [period, stats] : aggregated_data)
    {
        candlesticks.emplace_back(period, stats.open, stats.high, stats.low, stats.close,
                                  stats.total_volume, stats.total_volume / stats.count,
                                  stats.count);
    }

    CandlestickSummaryData data{candlesticks, product_pair, timeframe};

    return utils::ServiceResult<CandlestickSummaryData>::Success(data,
                                                                 "Summary generated successfully");
}

} // namespace services
