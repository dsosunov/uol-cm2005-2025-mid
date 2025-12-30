#include "services/trading_activities_service.hpp"

#include "app_constants.hpp"
#include "core/utils/time_utils.hpp"
#include "services/authentication_service.hpp"
#include "services/trading_service.hpp" // for OrderRecord + service methods
#include "services/wallet_service.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace services
{
std::vector<std::pair<std::string, std::string>> TradingActivitiesService::
    GetSimulationProductPairs() const
{
    std::vector<std::pair<std::string, std::string>> pairs;

    for (const auto& base : app::kSupportedCurrencies)
    {
        for (const auto& quote : app::kSupportedCurrencies)
        {
            if (base == quote)
            {
                continue;
            }
            pairs.emplace_back(base, quote);
        }
    }

    return pairs;
}

TradingActivitiesService::TradingActivitiesService(
    std::shared_ptr<AuthenticationService> auth_service,
    std::shared_ptr<WalletService> wallet_service, std::shared_ptr<TradingService> trading_service)
    : auth_service_(std::move(auth_service)), wallet_service_(std::move(wallet_service)),
      trading_service_(std::move(trading_service))
{
    if (!auth_service_)
    {
        throw std::invalid_argument("AuthenticationService is required");
    }
    if (!wallet_service_)
    {
        throw std::invalid_argument("WalletService is required");
    }
    if (!trading_service_)
    {
        throw std::invalid_argument("TradingService is required");
    }
}

utils::ServiceResult<TradingSimulationSummary> TradingActivitiesService::
    SimulateUserTradingActivities(int orders_per_side_per_pair)
{
    if (orders_per_side_per_pair <= 0)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "Orders per side must be positive");
    }

    auto user = auth_service_->GetAuthenticatedUser();
    if (!user.success)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(user.message);
    }

    int user_id = user.data->id;

    const auto simulation_pairs = GetSimulationProductPairs();
    if (simulation_pairs.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No simulation product pairs configured");
    }

    constexpr double kBaseSpread = 0.002;  // 0.2%
    constexpr double kStepSpread = 0.0005; // 0.05%
    constexpr double kAmount = 1.0;

    auto now = utils::Now();
    long long micros_offset = 0;

    std::vector<services::OrderRecord> new_orders;
    new_orders.reserve(simulation_pairs.size() * orders_per_side_per_pair * 2);

    struct WalletEffect
    {
        std::string base;
        std::string quote;
        bool is_bid;
        double price;
        double amount;
    };
    std::vector<WalletEffect> wallet_effects;
    wallet_effects.reserve(simulation_pairs.size() * orders_per_side_per_pair * 2);

    std::map<std::string, double, std::less<>> required;

    std::vector<std::string> supported_pairs;
    supported_pairs.reserve(simulation_pairs.size());

    for (const auto& [base, quote] : simulation_pairs)
    {
        double ref = app::SyntheticPrice(base, quote);
        if (!std::isfinite(ref) || ref <= 0.0)
        {
            continue;
        }

        std::string product_pair = base + "/" + quote;
        supported_pairs.push_back(product_pair);

        for (int i = 0; i < orders_per_side_per_pair; ++i)
        {
            double bid = ref * (1.0 - (kBaseSpread + i * kStepSpread));
            double ask = ref * (1.0 + (kBaseSpread + i * kStepSpread));

            bid = std::max(bid, ref * 0.0000001);
            ask = std::max(ask, ref * 0.0000001);

            services::OrderRecord bid_order;
            bid_order.product_pair = product_pair;
            bid_order.order_type = dto::OrderType::Bids;
            bid_order.price = bid;
            bid_order.amount = kAmount;
            bid_order.timestamp = now + std::chrono::microseconds{micros_offset++};
            new_orders.push_back(bid_order);
            wallet_effects.push_back(WalletEffect{base, quote, true, bid, kAmount});
            required[quote] += bid * kAmount;

            services::OrderRecord ask_order;
            ask_order.product_pair = product_pair;
            ask_order.order_type = dto::OrderType::Asks;
            ask_order.price = ask;
            ask_order.amount = kAmount;
            ask_order.timestamp = now + std::chrono::microseconds{micros_offset++};
            new_orders.push_back(ask_order);
            wallet_effects.push_back(WalletEffect{base, quote, false, ask, kAmount});
            required[base] += kAmount;
        }
    }

    if (supported_pairs.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No supported product pairs found for simulation");
    }

    if (new_orders.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No valid products/prices found to simulate");
    }

    // Require sufficient balances; no auto-funding.
    std::vector<std::string> missing;
    for (const auto& [currency, required_amount] : required)
    {
        if (required_amount <= 0.0)
        {
            continue;
        }

        auto bal = wallet_service_->GetBalance(user_id, currency);
        if (!bal.success)
        {
            return utils::ServiceResult<TradingSimulationSummary>::Failure(bal.message);
        }

        double have = bal.data.value_or(0.0);
        if (have + 1e-9 < required_amount)
        {
            missing.push_back(
                std::format("{} need {:.6f} have {:.6f}", currency, required_amount, have));
        }
    }

    if (!missing.empty())
    {
        std::string msg =
            "Insufficient balances for simulation. Deposit required currencies first.";
        for (const auto& line : missing)
        {
            msg += "\n";
            msg += line;
        }
        return utils::ServiceResult<TradingSimulationSummary>::Failure(msg);
    }

    // Apply wallet updates.
    for (const auto& effect : wallet_effects)
    {
        if (effect.is_bid)
        {
            double spend = effect.price * effect.amount;
            auto w = wallet_service_->Withdraw(user_id, effect.quote, spend);
            if (!w.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(w.message);
            }

            auto d = wallet_service_->Deposit(user_id, effect.base, effect.amount);
            if (!d.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(d.message);
            }
        }
        else
        {
            auto w = wallet_service_->Withdraw(user_id, effect.base, effect.amount);
            if (!w.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(w.message);
            }

            double receive = effect.price * effect.amount;
            auto d = wallet_service_->Deposit(user_id, effect.quote, receive);
            if (!d.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(d.message);
            }
        }
    }

    auto append = trading_service_->AppendOrders(new_orders);
    if (!append.success)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(append.message);
    }

    TradingSimulationSummary summary;
    summary.product_pairs = static_cast<int>(supported_pairs.size());
    summary.orders_created = static_cast<int>(new_orders.size());
    summary.orders_per_pair = orders_per_side_per_pair * 2;
    summary.simulated_pairs = supported_pairs;

    return utils::ServiceResult<TradingSimulationSummary>::Success(summary, "Simulation completed");
}

} // namespace services
