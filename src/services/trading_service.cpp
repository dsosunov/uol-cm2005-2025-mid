#include "services/trading_service.hpp"

#include "core/utils/date_filter.hpp"
#include "dto/constants.hpp"
#include "persistence/trading_data_adapter.hpp"

#include <algorithm>
#include <limits>
#include <map>

namespace services
{
namespace
{
struct PeriodStats
{
    double min_price = std::numeric_limits<double>::max();
    double max_price = std::numeric_limits<double>::lowest();
    double total_volume = 0.0;
    int count = 0;
};

void ProcessCandlestickOrder(const OrderRecord& order, std::string_view product_pair,
                             const utils::DateFilter& date_filter,
                             std::map<utils::TimePoint, CandlestickData>& candlestick_map)
{
    if (order.product_pair != product_pair)
        return;

    if (!date_filter.IsInRange(order.timestamp))
        return;

    auto& candle = candlestick_map[order.timestamp];
    if (candle.timestamp == utils::TimePoint{})
    {
        candle.timestamp = order.timestamp;
        candle.open = order.price;
        candle.high = order.price;
        candle.low = order.price;
        candle.close = order.price;
        candle.volume = 0.0;
    }
    else
    {
        candle.high = std::max(candle.high, order.price);
        candle.low = std::min(candle.low, order.price);
        candle.close = order.price;
    }

    candle.volume += order.amount;
}

void ProcessSummaryOrder(const OrderRecord& order, std::string_view product_pair,
                         const utils::DateFilter& date_filter, dto::Timeframe timeframe,
                         std::map<std::string, PeriodStats, std::less<>>& aggregated_data)
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
    stats.min_price = std::min(stats.min_price, order.price);
    stats.max_price = std::max(stats.max_price, order.price);
    stats.total_volume += order.amount;
    stats.count++;
}

} // namespace

TradingService::TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter)
    : adapter_(adapter)
{
}

utils::ServiceResult<std::vector<CandlestickData>> TradingService::GetCandlestickData(
    std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
    [[maybe_unused]] dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
    const std::optional<utils::TimePoint>& end_date) const
{
    std::string product_pair = std::string(currency_base) + "/" + std::string(currency_quote);
    std::map<utils::TimePoint, CandlestickData> candlestick_map;
    utils::DateFilter date_filter = utils::DateFilter::Create(start_date, end_date);

    adapter_->ReadWithProcessor(
        order_type, [&product_pair, &date_filter, &candlestick_map](const OrderRecord& order) {
            ProcessCandlestickOrder(order, product_pair, date_filter, candlestick_map);
        });

    if (candlestick_map.empty())
    {
        return utils::ServiceResult<std::vector<CandlestickData>>::Failure(
            "No data found for the specified currency pair and date range");
    }

    std::vector<CandlestickData> candlesticks;
    candlesticks.reserve(candlestick_map.size());

    for (const auto& [timestamp, candle] : candlestick_map)
    {
        candlesticks.push_back(candle);
    }

    return utils::ServiceResult<std::vector<CandlestickData>>::Success(candlesticks,
                                                                       "Data retrieved from CSV");
}

utils::ServiceResult<CandlestickSummaryData> TradingService::GetCandlestickSummary(
    std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
    dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
    const std::optional<utils::TimePoint>& end_date) const
{
    std::string product_pair = std::string(currency_base) + "/" + std::string(currency_quote);
    std::map<std::string, PeriodStats, std::less<>> aggregated_data;
    utils::DateFilter date_filter = utils::DateFilter::Create(start_date, end_date);

    adapter_->ReadWithProcessor(order_type, [&product_pair, &date_filter, &timeframe,
                                             &aggregated_data](const OrderRecord& order) {
        ProcessSummaryOrder(order, product_pair, date_filter, timeframe, aggregated_data);
    });

    if (aggregated_data.empty())
    {
        return utils::ServiceResult<CandlestickSummaryData>::Failure(
            "No data found for the specified currency pair and date range");
    }

    std::vector<PeriodSummary> periods;
    periods.reserve(aggregated_data.size());
    for (const auto& [period, stats] : aggregated_data)
    {
        periods.emplace_back(period, stats.min_price, stats.max_price, stats.total_volume,
                             stats.total_volume / stats.count, stats.count);
    }

    CandlestickSummaryData data{
        periods, std::string(currency_base) + "/" + std::string(currency_quote), timeframe};

    return utils::ServiceResult<CandlestickSummaryData>::Success(data,
                                                                 "Summary generated successfully");
}

utils::ServiceResult<GenerationData> TradingService::GenerateTrades(int count) const
{
    if (count <= 0)
    {
        return utils::ServiceResult<GenerationData>::Failure("Count must be positive");
    }

    std::map<std::string, int, std::less<>> trades_by_pair = {
        {"USD/EUR", count / 3},     {"GBP/USD", count / 5},  {"USD/JPY", count / 3},
        {"EUR/GBP", count / 5},     {"CAD/USD", count / 10}, {"AUD/USD", count / 10},
        {"USD/CHF", count / 10 + 1}};

    int total_generated = 0;
    for (const auto& [pair, cnt] : trades_by_pair)
    {
        total_generated += cnt;
    }

    double total_volume = count * 5000.0;
    GenerationData data{count, trades_by_pair, total_volume};

    return utils::ServiceResult<GenerationData>::Success(data, "Trades generated successfully");
}

utils::ServiceResult<std::set<std::string, std::less<>>> TradingService::GetAvailableProducts()
    const
{
    std::set<std::string, std::less<>> currencies;

    adapter_->ReadWithProcessor([&currencies](const OrderRecord& order) {
        const auto& product = order.product_pair;
        size_t slash_pos = product.find('/');

        if (slash_pos != std::string::npos)
        {
            currencies.insert(product.substr(0, slash_pos));
            currencies.insert(product.substr(slash_pos + 1));
        }
    });

    return {true, "Available products retrieved successfully", currencies};
}

utils::ServiceResult<std::vector<std::string>> TradingService::GetDateSamples(
    dto::Timeframe timeframe, const DateQueryOptions& options) const
{
    std::set<std::string, std::less<>> unique_dates;

    adapter_->ReadWithProcessor([&unique_dates, &options, &timeframe](const OrderRecord& order) {
        if (options.start_date.has_value() && order.timestamp < *options.start_date)
            return;

        if (options.end_date.has_value() && order.timestamp > *options.end_date)
            return;

        std::string formatted_date = utils::FormatDate(order.timestamp);

        if (timeframe == dto::Timeframe::Yearly && formatted_date.length() >= 4)
        {
            unique_dates.insert(formatted_date.substr(0, 4));
        }
        else if (timeframe == dto::Timeframe::Monthly && formatted_date.length() >= 7)
        {
            unique_dates.insert(formatted_date.substr(0, 7));
        }
        else if (formatted_date.length() >= 10)
        {
            unique_dates.insert(formatted_date.substr(0, 10));
        }
    });

    if (unique_dates.empty())
    {
        return {true, "No date samples found", {}};
    }

    std::vector<std::string> result(unique_dates.begin(), unique_dates.end());
    std::vector<std::string> filtered;
    int skipped = 0;

    for (const auto& date : result)
    {
        if (skipped < options.offset)
        {
            skipped++;
            continue;
        }

        filtered.push_back(date);

        if (options.limit.has_value() && filtered.size() >= static_cast<size_t>(*options.limit))
            break;
    }

    return {true, "Date samples retrieved successfully", filtered};
}

} // namespace services