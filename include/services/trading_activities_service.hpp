#pragma once

#include "core/utils/service_result.hpp"
#include "services/trading_service.hpp" // OrderRecord

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace services
{
class AuthenticationService;
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
        int orders_per_pair = 5) const;

  private:
    struct WalletEffect
    {
        std::string base;
        std::string quote;
        bool is_bid = false;
        double price = 0.0;
        double amount = 0.0;

        WalletEffect(std::string base_, std::string quote_, bool is_bid_, double price_,
                     double amount_)
            : base(std::move(base_)), quote(std::move(quote_)), is_bid(is_bid_), price(price_),
              amount(amount_)
        {
        }
    };

    double SyntheticPrice(std::string_view base, std::string_view quote) const;
    std::vector<std::pair<std::string, std::string>> GetSimulationProductPairs() const;

    utils::ServiceResult<int> GetAuthenticatedUserId() const;
    utils::ServiceResult<std::pair<std::string, std::string>> SelectPrimaryPair(
        const std::vector<std::pair<std::string, std::string>>& simulation_pairs) const;
    void GenerateOrdersAndEffects(
        const std::string& base, const std::string& quote, double reference_price,
        int orders_per_pair, std::vector<OrderRecord>& new_orders,
        std::vector<TradingActivitiesService::WalletEffect>& wallet_effects,
        std::vector<std::string>& supported_pairs) const;
    utils::ServiceResult<void> EnsureSufficientBalances(
        int user_id,
        const std::vector<TradingActivitiesService::WalletEffect>& wallet_effects) const;
    utils::ServiceResult<void> ApplyWalletEffects(
        int user_id,
        const std::vector<TradingActivitiesService::WalletEffect>& wallet_effects) const;
    utils::ServiceResult<void> AppendOrders(const std::vector<OrderRecord>& new_orders) const;

    std::shared_ptr<AuthenticationService> auth_service_;
    std::shared_ptr<WalletService> wallet_service_;
    std::shared_ptr<TradingService> trading_service_;
};

} // namespace services
