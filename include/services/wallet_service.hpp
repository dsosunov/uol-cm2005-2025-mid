#pragma once
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace data
{
    class CsvWriter;
}

namespace persistence
{
    class WalletDataAdapter;
}

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
        WalletService(std::shared_ptr<persistence::WalletDataAdapter> reader,
                      std::shared_ptr<data::CsvWriter> writer);
        ~WalletService() = default;
        std::map<std::string, double, std::less<>> GetBalances(std::optional<int> user_id = std::nullopt) const;
        double GetBalance(std::string_view currency,
                          std::optional<int> user_id = std::nullopt) const;
        double GetTotalBalanceInUSD(std::optional<int> user_id = std::nullopt) const;
        OperationResult Deposit(std::string_view currency, double amount,
                                std::optional<int> user_id = std::nullopt);
        OperationResult Withdraw(std::string_view currency, double amount,
                                 std::optional<int> user_id = std::nullopt);

    private:
        std::map<int, std::map<std::string, double, std::less<>>> balances_;
        int default_user_id_ = 1;
        std::shared_ptr<persistence::WalletDataAdapter> reader_;
        std::shared_ptr<data::CsvWriter> writer_;
        int GetEffectiveUserId(std::optional<int> user_id) const;
        void SaveBalances(int user_id);
    };

}
