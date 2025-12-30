#pragma once
#include "core/utils/service_result.hpp"

#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace services
{
class UserService;
class TransactionsService;
} // namespace services
namespace services
{
class WalletService
{
  public:
    WalletService() = delete;
    WalletService(std::shared_ptr<services::TransactionsService> transactions_service,
                  std::shared_ptr<services::UserService> user_service);
    ~WalletService() = default;
    utils::ServiceResult<std::map<std::string, double, std::less<>>> GetBalances(int user_id) const;
    utils::ServiceResult<double> GetBalance(int user_id, std::string_view currency) const;
    utils::ServiceResult<double> GetTotalBalanceInUSD(int user_id) const;
    utils::ServiceResult<double> Deposit(int user_id, std::string_view currency, double amount);
    utils::ServiceResult<double> Withdraw(int user_id, std::string_view currency, double amount);

  private:
    double ExchangeRateToUSD(std::string_view currency) const;
    std::shared_ptr<services::TransactionsService> transactions_service_;
    std::shared_ptr<services::UserService> user_service_;
    std::map<std::string, double, std::less<>> CalculateBalances(int user_id) const;
};
} // namespace services