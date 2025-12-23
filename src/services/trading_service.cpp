#include "services/trading_service.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "dto/constants.hpp"
#include <map>
#include <limits>
#include <algorithm>

namespace services
{

    TradingService::TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter)
        : adapter_(adapter)
    {
    }

    CandlestickResult TradingService::GetCandlestickData(std::string_view currency_base,
                                                         std::string_view currency_quote,
                                                         dto::OrderType order_type,
                                                         dto::Timeframe timeframe,
                                                         const std::optional<utils::TimePoint> &start_date,
                                                         const std::optional<utils::TimePoint> &end_date) const
    {
        std::string product_pair = std::string(currency_base) + "/" + std::string(currency_quote);

        // Business logic: collect orders and build candlesticks
        std::map<utils::TimePoint, CandlestickData> candlestick_map;

        // Use streaming to efficiently process large datasets
        adapter_->ReadWithProcessor(order_type, [&](const OrderRecord &order)
                                    {
            // Filter by product pair
            if (order.product_pair != product_pair)
                return;

            // Filter by date range - compare dates only (ignore time component)
            std::string order_date = utils::FormatDate(order.timestamp);

            if (start_date.has_value())
            {
                std::string start_date_str = utils::FormatDate(*start_date);
                if (order_date < start_date_str)
                    return;
            }
            if (end_date.has_value())
            {
                std::string end_date_str = utils::FormatDate(*end_date);
                if (order_date > end_date_str)
                    return;
            }

            // Business logic: aggregate into candlestick buckets
            // For now, use order timestamp as bucket (can be enhanced for time windows)
            auto& candle = candlestick_map[order.timestamp];
            if (candle.timestamp == utils::TimePoint{})
            {
                // Initialize new candlestick
                candle.timestamp = order.timestamp;
                candle.open = order.price;
                candle.high = order.price;
                candle.low = order.price;
                candle.close = order.price;
                candle.volume = 0.0;
            }
            else
            {
                // Update candlestick
                candle.high = std::max(candle.high, order.price);
                candle.low = std::min(candle.low, order.price);
                candle.close = order.price;
            }
            candle.volume += order.amount; });

        if (candlestick_map.empty())
        {
            return {{}, product_pair, timeframe, false, "No data found for the specified currency pair and date range"};
        }

        // Convert map to vector
        std::vector<CandlestickData> candlesticks;
        candlesticks.reserve(candlestick_map.size());
        for (const auto &[timestamp, candle] : candlestick_map)
        {
            candlesticks.push_back(candle);
        }

        return {candlesticks, product_pair, timeframe, true, "Data retrieved from CSV"};
    }

    CandlestickSummaryResult TradingService::GetCandlestickSummary(
        std::string_view currency_base,
        std::string_view currency_quote,
        dto::OrderType order_type,
        dto::Timeframe timeframe,
        const std::optional<utils::TimePoint> &start_date,
        const std::optional<utils::TimePoint> &end_date) const
    {
        std::string product_pair = std::string(currency_base) + "/" + std::string(currency_quote);

        // Business logic: collect and aggregate by period
        struct PeriodStats
        {
            double min_price = std::numeric_limits<double>::max();
            double max_price = std::numeric_limits<double>::lowest();
            double total_volume = 0.0;
            int count = 0;
        };

        std::map<std::string, PeriodStats> aggregated_data;

        // Use streaming to efficiently process large datasets
        adapter_->ReadWithProcessor(order_type, [&](const OrderRecord &order)
                                    {
            // Filter by product pair
            if (order.product_pair != product_pair)
            {
                return;
            }

            // Filter by date range - compare dates only (ignore time component)
            // This allows filtering by date to include the entire day
            std::string order_date = utils::FormatDate(order.timestamp);

            if (start_date.has_value())
            {
                std::string start_date_str = utils::FormatDate(*start_date);
                if (order_date < start_date_str)
                {
                    return;
                }
            }
            if (end_date.has_value())
            {
                std::string end_date_str = utils::FormatDate(*end_date);
                if (order_date > end_date_str)
                {
                    return;
                }
            }

            // Determine period key based on timeframe
            std::string period_key;
            std::string formatted_date = order_date;

            if (timeframe == dto::Timeframe::Daily && formatted_date.length() >= 10)
            {
                period_key = formatted_date.substr(0, 10); // YYYY-MM-DD
            }
            else if (timeframe == dto::Timeframe::Monthly && formatted_date.length() >= 7)
            {
                period_key = formatted_date.substr(0, 7); // YYYY-MM
            }
            else if (timeframe == dto::Timeframe::Yearly && formatted_date.length() >= 4)
            {
                period_key = formatted_date.substr(0, 4); // YYYY
            }
            else
            {
                period_key = formatted_date;
            }

            // Aggregate statistics
            auto &stats = aggregated_data[period_key];
            stats.min_price = std::min(stats.min_price, order.price);
            stats.max_price = std::max(stats.max_price, order.price);
            stats.total_volume += order.amount;
            stats.count++; });

        if (aggregated_data.empty())
        {
            return {{}, product_pair, timeframe, false, "No data found for the specified currency pair and date range"};
        }

        // Convert aggregated data to result format
        std::vector<PeriodSummary> periods;
        periods.reserve(aggregated_data.size());

        for (const auto &[period, stats] : aggregated_data)
        {
            periods.push_back({period,
                               stats.min_price,
                               stats.max_price,
                               stats.total_volume,
                               stats.total_volume / stats.count,
                               stats.count});
        }

        return {periods, product_pair, timeframe, true, "Summary generated successfully"};
    }

    GenerationResult TradingService::GenerateTrades(int count) const
    {
        if (count <= 0)
        {
            return {false, 0, {}, 0.0, "Count must be positive"};
        }
        std::map<std::string, int, std::less<>> trades_by_pair = {
            {"USD/EUR", count / 3}, {"GBP/USD", count / 5}, {"USD/JPY", count / 3}, {"EUR/GBP", count / 5}, {"CAD/USD", count / 10}, {"AUD/USD", count / 10}, {"USD/CHF", count / 10 + 1}};
        int total_generated = 0;
        for (const auto &[pair, cnt] : trades_by_pair)
        {
            total_generated += cnt;
        }
        double total_volume = count * 5000.0;

        return {true, count, trades_by_pair, total_volume,
                "Trades generated successfully"};
    }

    std::set<std::string, std::less<>> TradingService::GetAvailableProducts() const
    {
        std::set<std::string, std::less<>> currencies;

        // Use streaming to efficiently extract unique currencies
        adapter_->ReadWithProcessor([&currencies](const OrderRecord &order)
                                    {
            const auto& product = order.product_pair;
            size_t slash_pos = product.find('/');
            if (slash_pos != std::string::npos)
            {
                currencies.insert(product.substr(0, slash_pos));
                currencies.insert(product.substr(slash_pos + 1));
            } });

        return currencies;
    }

    std::vector<std::string> TradingService::GetDateSamples(
        dto::Timeframe timeframe,
        const DateQueryOptions &options) const
    {
        // Use streaming to efficiently extract unique dates
        std::set<std::string> unique_dates;

        adapter_->ReadWithProcessor([&](const OrderRecord &order)
                                    {
            // Filter by date range (inclusive of both start and end)
            if (options.start_date.has_value() && order.timestamp < *options.start_date)
                return;
            if (options.end_date.has_value() && order.timestamp > *options.end_date)
                return;

            // Aggregate by timeframe
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
            } });

        if (unique_dates.empty())
        {
            return {};
        }

        // Business logic: apply offset and limit
        std::vector<std::string> result(unique_dates.begin(), unique_dates.end());
        std::vector<std::string> filtered;
        int skipped = 0;

        for (const auto &date : result)
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

        return filtered;
    }
}
