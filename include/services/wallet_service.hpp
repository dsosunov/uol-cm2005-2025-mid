#pragma once
#include "core/utils/service_result.hpp"

#include <memory>

namespace services
{
class AuthenticationService;
}

#include <map>
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
    WalletService() = delete;
    WalletService(std::shared_ptr<persistence::TransactionDataAdapter> adapter,
                  std::shared_ptr<services::AuthenticationService> auth_service);
    ~WalletService() = default;
    utils::ServiceResult<std::map<std::string, double, std::less<>>> GetBalances() const;
    utils::ServiceResult<double> GetBalance(std::string_view currency) const;
    utils::ServiceResult<double> GetTotalBalanceInUSD() const;
    utils::ServiceResult<double> Deposit(std::string_view currency, double amount);
    utils::ServiceResult<double> Withdraw(std::string_view currency, double amount);

  private:
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
    std::map<std::string, double, std::less<>> CalculateBalances(int user_id) const;
};
} // namespace services