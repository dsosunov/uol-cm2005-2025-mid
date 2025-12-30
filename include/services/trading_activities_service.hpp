#pragma once

#include "core/utils/service_result.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace services
{
class AuthenticationService;
class TradingService;
class WalletService;

struct TradingSimulationSummary
{
    int product_pairs = 0;
    int orders_created = 0;
    int orders_per_pair = 0;
    std::vector<std::string> simulated_pairs;
};

class TradingActivitiesService
{
  public:
    TradingActivitiesService(std::shared_ptr<AuthenticationService> auth_service,
                             std::shared_ptr<WalletService> wallet_service,
                             std::shared_ptr<TradingService> trading_service);

    utils::ServiceResult<TradingSimulationSummary> SimulateUserTradingActivities(
        int orders_per_side_per_pair = 5);

  private:
    double SyntheticPrice(std::string_view base, std::string_view quote) const;
    std::vector<std::pair<std::string, std::string>> GetSimulationProductPairs() const;

    std::shared_ptr<AuthenticationService> auth_service_;
    std::shared_ptr<WalletService> wallet_service_;
    std::shared_ptr<TradingService> trading_service_;
};

} // namespace services
