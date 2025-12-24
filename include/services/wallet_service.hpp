#pragma once
#include "core/utils/service_result.hpp"

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
class WalletService
{
  public:
    WalletService();
    WalletService(std::shared_ptr<persistence::WalletDataAdapter> reader,
                  std::shared_ptr<data::CsvWriter> writer);
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
    mutable std::map<int, std::map<std::string, double, std::less<>>> balances_;
    int default_user_id_ = 1;
    std::shared_ptr<persistence::WalletDataAdapter> reader_;
    std::shared_ptr<data::CsvWriter> writer_;
    int GetEffectiveUserId(std::optional<int> user_id) const;
    void SaveBalances(int user_id);
};
} // namespace services