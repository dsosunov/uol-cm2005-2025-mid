#pragma once

#include "core/utils/service_result.hpp"

#include <memory>
#include <string>

namespace persistence
{
class TradingDataAdapter;
}

namespace services
{
class AuthenticationService;
class WalletService;

struct TradingSimulationSummary
{
    int product_pairs = 0;
    int orders_created = 0;
    int orders_per_pair = 0;
};

class TradingActivitiesService
{
  public:
    TradingActivitiesService(std::shared_ptr<AuthenticationService> auth_service,
                             std::shared_ptr<WalletService> wallet_service,
                             std::shared_ptr<persistence::TradingDataAdapter> trading_adapter);

    utils::ServiceResult<TradingSimulationSummary> SimulateUserTradingActivities(
        int orders_per_side_per_pair = 5);

  private:
    std::shared_ptr<AuthenticationService> auth_service_;
    std::shared_ptr<WalletService> wallet_service_;
    std::shared_ptr<persistence::TradingDataAdapter> trading_adapter_;
};

} // namespace services
