#pragma once

#include "core/utils/date_filter.hpp"
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace services
{
struct OrderRecord;
class TradingService;

struct Candlestick
{
    std::string period;
    double open;
    double high;
    double low;
    double close;
    double total_volume;
    double avg_volume;
    int trade_count;
};

struct CandlestickSummaryData
{
    std::vector<Candlestick> candlesticks;
    std::string product_pair;
    dto::Timeframe timeframe;
};

class AnalyticService
{
  public:
    explicit AnalyticService(std::shared_ptr<TradingService> trading_service);
    ~AnalyticService() = default;

    utils::ServiceResult<CandlestickSummaryData> GetCandlestickSummary(
        std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
        dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date) const;

  private:
    struct PeriodStats
    {
        utils::TimePoint first_timestamp{};
        utils::TimePoint last_timestamp{};
        double open = 0.0;
        double high = 0.0;
        double low = 0.0;
        double close = 0.0;
        double total_volume = 0.0;
        int count = 0;
    };

    void ProcessSummaryOrder(
        const OrderRecord& order, std::string_view product_pair,
        const utils::DateFilter& date_filter, dto::Timeframe timeframe,
        std::map<std::string, PeriodStats, std::less<>>& aggregated_data) const;

    std::shared_ptr<TradingService> trading_service_;
};
} // namespace services
