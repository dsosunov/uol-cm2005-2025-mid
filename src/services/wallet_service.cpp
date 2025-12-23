#include "services/wallet_service.hpp"

namespace services
{

    WalletService::WalletService() : default_user_id_(1)
    {
        // Pre-populate with some test balances for default user
        balances_[1]["USD"] = 1234.56;
        balances_[1]["EUR"] = 987.65;
        balances_[1]["GBP"] = 543.21;
        balances_[1]["JPY"] = 50000.00;
        balances_[1]["CAD"] = 1500.00;
    }

    int WalletService::GetEffectiveUserId(std::optional<int> user_id) const
    {
        return user_id.value_or(default_user_id_);
    }

    std::map<std::string, double> WalletService::GetBalances(std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        auto it = balances_.find(effective_id);
        if (it != balances_.end())
        {
            return it->second;
        }
        return {}; // Empty map if user has no balances
    }

    double WalletService::GetBalance(const std::string &currency, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        auto user_it = balances_.find(effective_id);
        if (user_it != balances_.end())
        {
            auto currency_it = user_it->second.find(currency);
            if (currency_it != user_it->second.end())
            {
                return currency_it->second;
            }
        }
        return 0.0;
    }

    double WalletService::GetTotalBalanceInUSD(std::optional<int> user_id) const
    {
        // Simplified conversion rates (hard-coded)
        std::map<std::string, double> rates = {
            {"USD", 1.0}, {"EUR", 1.09}, {"GBP", 1.27}, {"JPY", 0.0067}, {"CAD", 0.73}, {"AUD", 0.65}, {"CHF", 1.13}, {"CNY", 0.14}};

        int effective_id = GetEffectiveUserId(user_id);
        auto user_it = balances_.find(effective_id);
        if (user_it == balances_.end())
        {
            return 0.0;
        }

        double total_usd = 0.0;
        for (const auto &[currency, amount] : user_it->second)
        {
            auto rate_it = rates.find(currency);
            if (rate_it != rates.end())
            {
                total_usd += amount * rate_it->second;
            }
        }

        return total_usd;
    }

    OperationResult WalletService::Deposit(const std::string &currency, double amount,
                                           std::optional<int> user_id)
    {
        if (amount <= 0)
        {
            return {false, "Amount must be positive", 0.0};
        }

        int effective_id = GetEffectiveUserId(user_id);
        balances_[effective_id][currency] += amount;
        double new_balance = balances_[effective_id][currency];

        return {true, "Deposit successful", new_balance};
    }

    OperationResult WalletService::Withdraw(const std::string &currency, double amount,
                                            std::optional<int> user_id)
    {
        if (amount <= 0)
        {
            return {false, "Amount must be positive", 0.0};
        }

        int effective_id = GetEffectiveUserId(user_id);
        double current_balance = GetBalance(currency, user_id);

        if (current_balance < amount)
        {
            return {false, "Insufficient balance", current_balance};
        }

        balances_[effective_id][currency] -= amount;
        double new_balance = balances_[effective_id][currency];

        return {true, "Withdrawal successful", new_balance};
    }

} // namespace services
