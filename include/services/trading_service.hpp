#pragma once
#include "core/utils/service_result.hpp"
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
struct CandlestickMetadata
{
    std::string product_pair;
    dto::Timeframe timeframe;
};
struct GenerationData
{
    int trades_generated;
    std::map<std::string, int, std::less<>> trades_by_pair;
    double total_volume;

    GenerationData() = default;
    GenerationData(int trades_generated, std::map<std::string, int, std::less<>> trades_by_pair,
                   double total_volume)
        : trades_generated(trades_generated), trades_by_pair(std::move(trades_by_pair)),
          total_volume(total_volume)
    {
    }
    GenerationData(GenerationData&&) noexcept = default;
    GenerationData& operator=(GenerationData&&) noexcept = default;
    GenerationData(const GenerationData&) = default;
    GenerationData& operator=(const GenerationData&) = default;
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
    std::vector<PeriodSummary> periods;
    std::string product_pair;
    dto::Timeframe timeframe;
};
class TradingService
{
  public:
    explicit TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter);
    ~TradingService() = default;
    utils::ServiceResult<CandlestickSummaryData> GetCandlestickSummary(
        std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
        dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date) const;
    utils::ServiceResult<GenerationData> GenerateTrades(int count) const;
    utils::ServiceResult<std::set<std::string, std::less<>>> GetAvailableProducts() const;
    utils::ServiceResult<std::vector<std::string>> GetDateSamples(
        dto::Timeframe timeframe, const DateQueryOptions& options) const;

  private:
    std::shared_ptr<persistence::TradingDataAdapter> adapter_;
};
} // namespace services