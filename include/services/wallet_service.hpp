#pragma once
#include <map>
#include <optional>
#include <string>
#include <string_view>

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
        std::map<std::string, double, std::less<>> GetBalances(std::optional<int> user_id = std::nullopt) const;
        double GetBalance(std::string_view currency,
                          std::optional<int> user_id = std::nullopt) const;
        double GetTotalBalanceInUSD(std::optional<int> user_id = std::nullopt) const;

        // Transaction operations
        OperationResult Deposit(std::string_view currency, double amount,
                                std::optional<int> user_id = std::nullopt);
        OperationResult Withdraw(std::string_view currency, double amount,
                                 std::optional<int> user_id = std::nullopt);

    private:
        // Map of user_id -> currency -> balance
        std::map<int, std::map<std::string, double, std::less<>>> balances_;
        int default_user_id_ = 1;

        // Helper to get effective user ID
        int GetEffectiveUserId(std::optional<int> user_id) const;
    };

} // namespace services
