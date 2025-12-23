#pragma once
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace services
{

    struct CandlestickData
    {
        std::string timestamp;
        double open;
        double high;
        double low;
        double close;
        double volume;
    };

    struct CandlestickResult
    {
        std::vector<CandlestickData> data;
        std::string product_pair;
        std::string timeframe;
        bool success;
        std::string message;
    };

    struct GenerationResult
    {
        bool success;
        int trades_generated;
        std::map<std::string, int, std::less<>> trades_by_pair;
        double total_volume;
        std::string message;
    };

    /**
     * @brief Options for querying date samples with server-side filtering
     * Enables lazy loading by only fetching needed data from source
     */
    struct DateQueryOptions
    {
        std::optional<std::string> start_date; // Filter: date >= start_date
        std::optional<std::string> end_date;   // Filter: date <= end_date
        std::optional<int> limit;              // Max results to return (pagination)
        int offset = 0;                        // Skip N results (pagination)
    };

    class TradingService
    {
    public:
        TradingService() = default;
        ~TradingService() = default;

        // Market data operations
        CandlestickResult GetCandlestickData(std::string_view currency_base,
                                             std::string_view currency_quote,
                                             std::string_view asks_bids,
                                             std::string_view timeframe,
                                             std::string_view start_date,
                                             std::string_view end_date) const;

        // Trade generation
        GenerationResult GenerateTrades(int count) const;

        // Utility
        std::set<std::string, std::less<>> GetAvailableCurrencies() const;

        /**
         * @brief Get date samples with server-side filtering (lazy loading)
         * @param timeframe "daily", "monthly", or "yearly"
         * @param options Query filters (start_date, end_date, limit, offset)
         * @return Filtered date strings (only requested subset, not all data)
         */
        std::vector<std::string> GetDateSamples(std::string_view timeframe,
                                                const DateQueryOptions &options) const;

    private:
        std::set<std::string, std::less<>> available_currencies_ = {"USD", "CAD", "EUR", "GBP", "JPY", "AUD", "CHF", "CNY"};
    };

} // namespace services
