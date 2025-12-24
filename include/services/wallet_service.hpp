#pragma once
#include "core/utils/service_result.hpp"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace persistence
{
class TransactionDataAdapter;
}
namespace services
{
class WalletService
{
  public:
    WalletService() = default;
    explicit WalletService(std::shared_ptr<persistence::TransactionDataAdapter> adapter);
    ~WalletService() = default;
    utils::ServiceResult<std::map<std::string, double, std::less<>>> GetBalances(
        std::optional<int> user_id = std::nullopt) const;
    utils::ServiceResult<double> GetBalance(std::string_view currency,
                                            std::optional<int> user_id = std::nullopt) const;
    utils::ServiceResult<double> GetTotalBalanceInUSD(
        std::optional<int> user_id = std::nullopt) const;
    utils::ServiceResult<double> Deposit(std::string_view currency, double amount,
                                         std::optional<int> user_id = std::nullopt);
    utils::ServiceResult<double> Withdraw(std::string_view currency, double amount,
                                          std::optional<int> user_id = std::nullopt);

  private:
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    int default_user_id_ = 1;
    int GetEffectiveUserId(std::optional<int> user_id) const;
    std::map<std::string, double, std::less<>> CalculateBalances(int user_id) const;
};
} // namespace services