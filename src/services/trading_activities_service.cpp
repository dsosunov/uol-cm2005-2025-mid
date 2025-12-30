#include "services/trading_activities_service.hpp"

#include "app_constants.hpp"
#include "core/utils/time_utils.hpp"
#include "services/authentication_service.hpp"
#include "services/trading_service.hpp" // for OrderRecord + service methods
#include "services/wallet_service.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <string>
#include <string_view>
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

double TradingActivitiesService::SyntheticPrice(std::string_view base, std::string_view quote) const
{
    auto it_base = app::kSyntheticPriceInUSDT.find(std::string(base));
    auto it_quote = app::kSyntheticPriceInUSDT.find(std::string(quote));
    if (it_base == app::kSyntheticPriceInUSDT.end() ||
        it_quote == app::kSyntheticPriceInUSDT.end() || it_quote->second <= 0.0)
    {
        return 0.0;
    }

    return it_base->second / it_quote->second;
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

utils::ServiceResult<int> TradingActivitiesService::GetAuthenticatedUserId() const
{
    auto user = auth_service_->GetAuthenticatedUser();
    if (!user.success || !user.data)
    {
        return utils::ServiceResult<int>::Failure(user.message);
    }

    return utils::ServiceResult<int>::Success(user.data->id);
}

utils::ServiceResult<std::pair<std::string, std::string>> TradingActivitiesService::
    SelectPrimaryPair(
        const std::vector<std::pair<std::string, std::string>>& simulation_pairs) const
{
    for (const auto& [base, quote] : simulation_pairs)
    {
        double ref = SyntheticPrice(base, quote);
        if (std::isfinite(ref) && ref > 0.0)
        {
            return utils::ServiceResult<std::pair<std::string, std::string>>::Success(
                {base, quote});
        }
    }

    return utils::ServiceResult<std::pair<std::string, std::string>>::Failure(
        "No supported product pairs found for simulation");
}

void TradingActivitiesService::GenerateOrdersAndEffects(
    const std::string& base, const std::string& quote, double reference_price, int orders_per_pair,
    std::vector<services::OrderRecord>& new_orders,
    std::vector<TradingActivitiesService::WalletEffect>& wallet_effects,
    std::vector<std::string>& supported_pairs) const
{
    supported_pairs.reserve(1);
    new_orders.reserve(static_cast<size_t>(orders_per_pair));
    wallet_effects.reserve(static_cast<size_t>(orders_per_pair));

    std::string product_pair = base + "/" + quote;
    supported_pairs.emplace_back(product_pair);

    constexpr double kBaseSpread = 0.002;  // 0.2%
    constexpr double kStepSpread = 0.0005; // 0.05%

    // Vary amount (and price) deterministically.
    constexpr double kBaseAmount = 1.0;
    constexpr double kAmountStep = 0.1; // +10% each level

    auto now = utils::Now();
    long long micros_offset = 0;

    // Generate up to `orders_per_pair` orders by alternating bid/ask per level.
    const int levels = (orders_per_pair + 1) / 2;
    int created = 0;
    for (int i = 0; i < levels && created < orders_per_pair; ++i)
    {
        const double level_spread = kBaseSpread + i * kStepSpread;
        double bid = reference_price * (1.0 - level_spread);
        double ask = reference_price * (1.0 + level_spread);

        bid = std::max(bid, reference_price * 0.0000001);
        ask = std::max(ask, reference_price * 0.0000001);

        const double amount = kBaseAmount * (1.0 + i * kAmountStep);

        if (created < orders_per_pair)
        {
            new_orders.emplace_back(services::OrderRecord{
                product_pair, now + std::chrono::microseconds{micros_offset++},
                dto::OrderType::Bids, bid, amount});
            wallet_effects.emplace_back(base, quote, true, bid, amount);
            ++created;
        }

        if (created < orders_per_pair)
        {
            new_orders.emplace_back(services::OrderRecord{
                product_pair, now + std::chrono::microseconds{micros_offset++},
                dto::OrderType::Asks, ask, amount});
            wallet_effects.emplace_back(base, quote, false, ask, amount);
            ++created;
        }
    }
}

utils::ServiceResult<void> TradingActivitiesService::EnsureSufficientBalances(
    int user_id, const std::vector<TradingActivitiesService::WalletEffect>& wallet_effects) const
{
    // Require sufficient balances; no auto-funding.
    // Important: a bid immediately deposits `base` before the corresponding ask withdraws it.
    // So we validate against the *sequential* wallet effects, not by summing all bids/asks
    // up-front.
    auto balances_result = wallet_service_->GetBalances(user_id);
    if (!balances_result.success || !balances_result.data.has_value())
    {
        return utils::ServiceResult<void>::Failure(
            balances_result.success ? "Failed to retrieve balances" : balances_result.message);
    }

    auto starting_balances = *balances_result.data;
    auto running_balances = starting_balances;
    auto min_balances = starting_balances;

    for (const auto& effect : wallet_effects)
    {
        if (effect.is_bid)
        {
            const double spend = effect.price * effect.amount;
            running_balances[effect.quote] -= spend;
            running_balances[effect.base] += effect.amount;
        }
        else
        {
            running_balances[effect.base] -= effect.amount;
            const double receive = effect.price * effect.amount;
            running_balances[effect.quote] += receive;
        }

        for (const auto& [currency, value] : running_balances)
        {
            auto it = min_balances.find(currency);
            if (it == min_balances.end())
            {
                min_balances.try_emplace(currency, value);
            }
            else
            {
                it->second = std::min(it->second, value);
            }
        }
    }

    constexpr double kEpsilon = 1e-9;
    std::vector<std::string> missing;
    for (const auto& [currency, min_value] : min_balances)
    {
        if (min_value >= -kEpsilon)
        {
            continue;
        }

        const double have =
            starting_balances.contains(currency) ? starting_balances[currency] : 0.0;
        const double required_start = have + (-min_value);
        missing.push_back(
            std::format("{} need {:.6f} have {:.6f}", currency, required_start, have));
    }

    if (missing.empty())
    {
        return utils::ServiceResult<void>::Success();
    }

    std::string msg = "Insufficient balances for simulation. Deposit required currencies first.";
    for (const auto& line : missing)
    {
        msg += "\n";
        msg += line;
    }

    return utils::ServiceResult<void>::Failure(msg);
}

utils::ServiceResult<void> TradingActivitiesService::ApplyWalletEffects(
    int user_id, const std::vector<TradingActivitiesService::WalletEffect>& wallet_effects) const
{
    // Apply wallet updates.
    for (const auto& effect : wallet_effects)
    {
        if (effect.is_bid)
        {
            double spend = effect.price * effect.amount;
            if (auto w = wallet_service_->Withdraw(user_id, effect.quote, spend); !w.success)
            {
                return utils::ServiceResult<void>::Failure(w.message);
            }

            if (auto d = wallet_service_->Deposit(user_id, effect.base, effect.amount); !d.success)
            {
                return utils::ServiceResult<void>::Failure(d.message);
            }
        }
        else
        {
            if (auto w = wallet_service_->Withdraw(user_id, effect.base, effect.amount); !w.success)
            {
                return utils::ServiceResult<void>::Failure(w.message);
            }

            double receive = effect.price * effect.amount;
            if (auto d = wallet_service_->Deposit(user_id, effect.quote, receive); !d.success)
            {
                return utils::ServiceResult<void>::Failure(d.message);
            }
        }
    }

    return utils::ServiceResult<void>::Success();
}

utils::ServiceResult<void> TradingActivitiesService::AppendOrders(
    const std::vector<services::OrderRecord>& new_orders) const
{
    if (auto append = trading_service_->AppendOrders(new_orders); !append.success)
    {
        return utils::ServiceResult<void>::Failure(append.message);
    }

    return utils::ServiceResult<void>::Success();
}

utils::ServiceResult<TradingSimulationSummary> TradingActivitiesService::
    SimulateUserTradingActivities(int orders_per_pair) const
{
    if (orders_per_pair <= 0)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "Orders per pair must be positive");
    }

    auto user_id_result = GetAuthenticatedUserId();
    if (!user_id_result.success || !user_id_result.data.has_value())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(user_id_result.message);
    }
    const int user_id = *user_id_result.data;

    const auto simulation_pairs = GetSimulationProductPairs();
    if (simulation_pairs.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No simulation product pairs configured");
    }

    auto primary_pair_result = SelectPrimaryPair(simulation_pairs);
    if (!primary_pair_result.success || !primary_pair_result.data.has_value())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(primary_pair_result.message);
    }

    const auto& base = primary_pair_result.data->first;
    const auto& quote = primary_pair_result.data->second;
    const double ref = SyntheticPrice(base, quote);

    std::vector<services::OrderRecord> new_orders;
    std::vector<WalletEffect> wallet_effects;
    std::vector<std::string> supported_pairs;
    GenerateOrdersAndEffects(base, quote, ref, orders_per_pair, new_orders, wallet_effects,
                             supported_pairs);

    if (new_orders.empty())
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(
            "No valid products/prices found to simulate");
    }

    if (auto balances_ok = EnsureSufficientBalances(user_id, wallet_effects); !balances_ok.success)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(balances_ok.message);
    }

    if (auto apply_ok = ApplyWalletEffects(user_id, wallet_effects); !apply_ok.success)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(apply_ok.message);
    }

    if (auto append_ok = AppendOrders(new_orders); !append_ok.success)
    {
        return utils::ServiceResult<TradingSimulationSummary>::Failure(append_ok.message);
    }

    TradingSimulationSummary summary;
    summary.product_pairs = static_cast<int>(supported_pairs.size());
    summary.orders_created = static_cast<int>(new_orders.size());
    summary.orders_per_pair = summary.product_pairs > 0
                                  ? (summary.orders_created / summary.product_pairs)
                                  : summary.orders_created;
    summary.simulated_pairs = supported_pairs;

    return utils::ServiceResult<TradingSimulationSummary>::Success(summary, "Simulation completed");
}

} // namespace services
