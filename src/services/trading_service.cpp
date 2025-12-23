#include "services/trading_service.hpp"

namespace services
{

    TradingService::TradingService()
    {
        // Initialize available currencies
        available_currencies_ = {"USD", "CAD", "EUR", "GBP", "JPY", "AUD", "CHF", "CNY"};
    }

    CandlestickResult TradingService::GetCandlestickData(const std::string &currency_base,
                                                         const std::string &currency_quote,
                                                         const std::string &asks_bids,
                                                         const std::string &timeframe,
                                                         const std::string &start_date,
                                                         const std::string &end_date) const
    {
        std::string product_pair = currency_base + "/" + currency_quote;

        // Generate some mock candlestick data
        std::vector<CandlestickData> data;

        // Create 5 sample candlesticks with realistic-looking data
        data.push_back({"2025-12-22 10:00", 1.2450, 1.2485, 1.2440, 1.2470, 125000.0});
        data.push_back({"2025-12-22 11:00", 1.2470, 1.2510, 1.2455, 1.2495, 145000.0});
        data.push_back({"2025-12-22 12:00", 1.2495, 1.2520, 1.2480, 1.2505, 135000.0});
        data.push_back({"2025-12-22 13:00", 1.2505, 1.2530, 1.2490, 1.2515, 155000.0});
        data.push_back({"2025-12-22 14:00", 1.2515, 1.2545, 1.2505, 1.2530, 165000.0});

        return {data, product_pair, timeframe, true, "Data retrieved successfully"};
    }

    GenerationResult TradingService::GenerateTrades(int count)
    {
        if (count <= 0)
        {
            return {false, 0, {}, 0.0, "Count must be positive"};
        }

        // Simulate trade generation
        std::map<std::string, int> trades_by_pair = {
            {"USD/EUR", count / 3}, {"GBP/USD", count / 5}, {"USD/JPY", count / 3}, {"EUR/GBP", count / 5}, {"CAD/USD", count / 10}, {"AUD/USD", count / 10}, {"USD/CHF", count / 10 + 1}};

        // Adjust to match exact count
        int total_generated = 0;
        for (const auto &[pair, cnt] : trades_by_pair)
        {
            total_generated += cnt;
        }

        // Simulate volume
        double total_volume = count * 5000.0; // $5,000 per trade on average

        return {true, count, trades_by_pair, total_volume,
                "Trades generated successfully"};
    }

    std::set<std::string, std::less<>> TradingService::GetAvailableCurrencies() const
    {
        return available_currencies_;
    }

    std::vector<std::string> TradingService::GetDateSamples(
        const std::string &timeframe,
        const DateQueryOptions &options) const
    {
        // Simulate database with huge datasets
        // In reality: SELECT date FROM market_data WHERE timeframe=? AND date>=? AND date<=? LIMIT ? OFFSET ?

        std::vector<std::string> all_dates;

        // Generate full dataset based on timeframe (simulating database table)
        if (timeframe == "monthly")
        {
            all_dates = {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05", "2025-06",
                         "2025-07", "2025-08", "2025-09", "2025-10", "2025-11", "2025-12"};
        }
        else if (timeframe == "yearly")
        {
            all_dates = {"2020", "2021", "2022", "2023", "2024", "2025"};
        }
        else // daily
        {
            all_dates = {"2025-12-01", "2025-12-15", "2025-12-22"};
        }

        // Apply server-side filtering (simulating WHERE clause)
        std::vector<std::string> filtered;
        int skipped = 0;

        for (const auto &date : all_dates)
        {
            // Filter: >= start_date
            if (options.start_date.has_value() && date < *options.start_date)
                continue;

            // Filter: <= end_date
            if (options.end_date.has_value() && date > *options.end_date)
                continue;

            // Skip offset records (pagination)
            if (skipped < options.offset)
            {
                skipped++;
                continue;
            }

            filtered.push_back(date);

            // Apply limit - STOP EARLY, don't load everything!
            if (options.limit.has_value() && filtered.size() >= static_cast<size_t>(*options.limit))
                break;
        }

        return filtered; // Only return requested subset
    }

} // namespace services
