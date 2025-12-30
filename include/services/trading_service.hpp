#pragma once
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/date_query_options.hpp"

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
class TradingService
{
  public:
    explicit TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter);
    ~TradingService() = default;
    utils::ServiceResult<CandlestickSummaryData> GetCandlestickSummary(
        std::string_view currency_base, std::string_view currency_quote, dto::OrderType order_type,
        dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date) const;
    utils::ServiceResult<std::set<std::string, std::less<>>> GetAvailableProducts() const;
    utils::ServiceResult<std::vector<std::string>> GetDateSamples(
        dto::Timeframe timeframe, const DateQueryOptions& options) const;
    utils::ServiceResult<void> AppendOrders(const std::vector<OrderRecord>& orders) const;

  private:
    std::shared_ptr<persistence::TradingDataAdapter> adapter_;
};
} // namespace services