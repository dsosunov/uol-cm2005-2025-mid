#pragma once
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/date_query_options.hpp"

#include <functional>
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
class TradingService
{
  public:
    explicit TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter);
    ~TradingService() = default;
    utils::ServiceResult<void> ForEachOrder(
        dto::OrderType order_type, const std::function<void(const OrderRecord&)>& processor) const;
    utils::ServiceResult<std::set<std::string, std::less<>>> GetAvailableProducts() const;
    utils::ServiceResult<std::vector<std::string>> GetDateSamples(
        dto::Timeframe timeframe, const DateQueryOptions& options) const;
    utils::ServiceResult<void> AppendOrders(const std::vector<OrderRecord>& orders) const;

  private:
    std::shared_ptr<persistence::TradingDataAdapter> adapter_;
};
} // namespace services