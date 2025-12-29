#include "services/trading_activities_service.hpp"

#include "core/utils/time_utils.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "services/authentication_service.hpp"
#include "services/trading_service.hpp" // for OrderRecord
#include "services/wallet_service.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace services
{
namespace
{
struct ParsedPair
{
    std::string base;
    std::string quote;
};

std::optional<ParsedPair> SplitPair(std::string_view product_pair)
{
    auto slash = product_pair.find('/');
    if (slash == std::string_view::npos)
    {
        return std::nullopt;
    }

    ParsedPair out{std::string(product_pair.substr(0, slash)),
                   std::string(product_pair.substr(slash + 1))};

    if (out.base.empty() || out.quote.empty())
    {
        return std::nullopt;
    }

    return out;
}
} // namespace

TradingActivitiesService::TradingActivitiesService(
    std::shared_ptr<AuthenticationService> auth_service,
    std::shared_ptr<WalletService> wallet_service,
    std::shared_ptr<persistence::TradingDataAdapter> trading_adapter)
    : auth_service_(std::move(auth_service)), wallet_service_(std::move(wallet_service)),
      trading_adapter_(std::move(trading_adapter))
{
    if (!auth_service_)
    {
        throw std::invalid_argument("AuthenticationService is required");
    }
    if (!wallet_service_)
    {
        throw std::invalid_argument("WalletService is required");
    }
    if (!trading_adapter_)
    {
        throw std::invalid_argument("TradingDataAdapter is required");
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

    std::set<std::string, std::less<>> product_pairs;
    std::map<std::string, services::OrderRecord, std::less<>> latest_by_pair;

    trading_adapter_->ReadWithProcessor([&](const services::OrderRecord& order) {
        product_pairs.insert(order.product_pair);

        auto it = latest_by_pair.find(order.product_pair);
        if (it == latest_by_pair.end() || order.timestamp > it->second.timestamp)
        {
            latest_by_pair[order.product_pair] = order;
        }
    });

    if (product_pairs.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No products found in trading CSV");
    }

    constexpr double kBaseSpread = 0.002;  // 0.2%
    constexpr double kStepSpread = 0.0005; // 0.05%
    constexpr double kAmount = 1.0;

    const std::set<std::string, std::less<>> allowed_currencies = {"BTC", "DOGE", "ETH", "USDT"};

    auto now = utils::Now();
    long long micros_offset = 0;

    std::vector<services::OrderRecord> new_orders;
    new_orders.reserve(product_pairs.size() * orders_per_side_per_pair * 2);

    struct WalletEffect
    {
        std::string base;
        std::string quote;
        bool is_bid;
        double price;
        double amount;
    };
    std::vector<WalletEffect> wallet_effects;
    wallet_effects.reserve(product_pairs.size() * orders_per_side_per_pair * 2);

    std::map<std::string, double, std::less<>> required;

    std::set<std::string, std::less<>> supported_pairs;

    for (const auto& pair : product_pairs)
    {
        auto parsed = SplitPair(pair);
        if (!parsed.has_value())
        {
            continue;
        }

        if (!allowed_currencies.contains(parsed->base) ||
            !allowed_currencies.contains(parsed->quote))
        {
            continue;
        }

        auto it = latest_by_pair.find(pair);
        if (it == latest_by_pair.end())
        {
            continue;
        }

        double ref = it->second.price;
        if (!std::isfinite(ref) || ref <= 0.0)
        {
            continue;
        }

        supported_pairs.insert(pair);

        for (int i = 0; i < orders_per_side_per_pair; ++i)
        {
            double bid = ref * (1.0 - (kBaseSpread + i * kStepSpread));
            double ask = ref * (1.0 + (kBaseSpread + i * kStepSpread));

            bid = std::max(bid, ref * 0.0000001);
            ask = std::max(ask, ref * 0.0000001);

            services::OrderRecord bid_order;
            bid_order.product_pair = pair;
            bid_order.order_type = dto::OrderType::Bids;
            bid_order.price = bid;
            bid_order.amount = kAmount;
            bid_order.timestamp = now + std::chrono::microseconds{micros_offset++};
            new_orders.push_back(bid_order);
            wallet_effects.push_back(WalletEffect{parsed->base, parsed->quote, true, bid, kAmount});
            required[parsed->quote] += bid * kAmount;

            services::OrderRecord ask_order;
            ask_order.product_pair = pair;
            ask_order.order_type = dto::OrderType::Asks;
            ask_order.price = ask;
            ask_order.amount = kAmount;
            ask_order.timestamp = now + std::chrono::microseconds{micros_offset++};
            new_orders.push_back(ask_order);
            wallet_effects.push_back(
                WalletEffect{parsed->base, parsed->quote, false, ask, kAmount});
            required[parsed->base] += kAmount;
        }
    }

    if (supported_pairs.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No supported product pairs found for simulation (BTC/DOGE/ETH/USDT only)");
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

        auto bal = wallet_service_->GetBalance(currency);
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
            auto w = wallet_service_->Withdraw(effect.quote, spend);
            if (!w.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(w.message);
            }

            auto d = wallet_service_->Deposit(effect.base, effect.amount);
            if (!d.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(d.message);
            }
        }
        else
        {
            auto w = wallet_service_->Withdraw(effect.base, effect.amount);
            if (!w.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(w.message);
            }

            double receive = effect.price * effect.amount;
            auto d = wallet_service_->Deposit(effect.quote, receive);
            if (!d.success)
            {
                return utils::ServiceResult<TradingSimulationSummary>::Failure(d.message);
            }
        }
    }

    if (!trading_adapter_->AddAll(new_orders))
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "Failed to append simulated orders");
    }

    TradingSimulationSummary summary;
    summary.product_pairs = static_cast<int>(supported_pairs.size());
    summary.orders_created = static_cast<int>(new_orders.size());
    summary.orders_per_pair = orders_per_side_per_pair * 2;

    return utils::ServiceResult<TradingSimulationSummary>::Success(summary, "Simulation completed");
}

} // namespace services
