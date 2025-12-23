#pragma once
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace persistence
{
    class TradingDataAdapter;
}

namespace services
{
    // Raw order/trade record from CSV (what adapter returns)
    struct OrderRecord
    {
        std::string product_pair;
        utils::TimePoint timestamp;
        dto::OrderType order_type;
        double price;
        double amount;
    };

    struct CandlestickData
    {
        utils::TimePoint timestamp;
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
        dto::Timeframe timeframe;
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

    struct DateQueryOptions
    {
        std::optional<utils::TimePoint> start_date;
        std::optional<utils::TimePoint> end_date;
        std::optional<int> limit;
        int offset = 0;
    };

    struct PeriodSummary
    {
        std::string period;
        double min_price;
        double max_price;
        double total_volume;
        double avg_volume;
        int trade_count;
    };

    struct CandlestickSummaryResult
    {
        std::vector<PeriodSummary> periods;
        std::string product_pair;
        dto::Timeframe timeframe;
        bool success;
        std::string message;
    };

    class TradingService
    {
    public:
        explicit TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter);
        ~TradingService() = default;
        CandlestickResult GetCandlestickData(std::string_view currency_base,
                                             std::string_view currency_quote,
                                             dto::OrderType order_type,
                                             dto::Timeframe timeframe,
                                             const std::optional<utils::TimePoint> &start_date,
                                             const std::optional<utils::TimePoint> &end_date) const;

        CandlestickSummaryResult GetCandlestickSummary(std::string_view currency_base,
                                                       std::string_view currency_quote,
                                                       dto::OrderType order_type,
                                                       dto::Timeframe timeframe,
                                                       const std::optional<utils::TimePoint> &start_date,
                                                       const std::optional<utils::TimePoint> &end_date) const;

        GenerationResult GenerateTrades(int count) const;
        std::set<std::string, std::less<>> GetAvailableProducts() const;

        std::vector<std::string> GetDateSamples(dto::Timeframe timeframe,
                                                const DateQueryOptions &options) const;

    private:
        std::shared_ptr<persistence::TradingDataAdapter> adapter_;
    };

}
