#pragma once
#include <map>
#include <optional>
#include <string>

namespace services
{

    struct OperationResult
    {
        bool success;
        std::string message;
        double new_balance;
    };

    class WalletService
    {
    public:
        WalletService();
        ~WalletService() = default;

        // Balance operations
        std::map<std::string, double> GetBalances(std::optional<int> user_id = std::nullopt) const;
        double GetBalance(const std::string &currency,
                          std::optional<int> user_id = std::nullopt) const;
        double GetTotalBalanceInUSD(std::optional<int> user_id = std::nullopt) const;

        // Transaction operations
        OperationResult Deposit(const std::string &currency, double amount,
                                std::optional<int> user_id = std::nullopt);
        OperationResult Withdraw(const std::string &currency, double amount,
                                 std::optional<int> user_id = std::nullopt);

    private:
        // Map of user_id -> currency -> balance
        std::map<int, std::map<std::string, double>> balances_;
        int default_user_id_;

        // Helper to get effective user ID
        int GetEffectiveUserId(std::optional<int> user_id) const;
    };

} // namespace services
