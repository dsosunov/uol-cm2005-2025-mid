#pragma once
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/date_query_options.hpp"

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace services
{
class UserService;
}

namespace persistence
{
class TransactionDataAdapter;
}
namespace services
{
// Wallet transaction type values persisted in CSV.
inline constexpr std::string_view kWalletTransactionTypeDeposit = "Deposit";
inline constexpr std::string_view kWalletTransactionTypeWithdraw = "Withdraw";

struct WalletTransaction
{
    std::string currency;
    std::string type; // One of kWalletTransactionTypeDeposit / kWalletTransactionTypeWithdraw
    double amount;
    utils::TimePoint timestamp;
    int user_id;
};

struct ActivityStats
{
    int total_transactions;
    double total_volume;
    double average_transaction_size;
};
class TransactionsService
{
  public:
    TransactionsService(std::shared_ptr<persistence::TransactionDataAdapter> adapter,
                        std::shared_ptr<services::UserService> user_service);
    ~TransactionsService() = default;
    utils::ServiceResult<std::vector<WalletTransaction>> GetLastTransactions(int user_id,
                                                                             int count) const;
    utils::ServiceResult<std::vector<WalletTransaction>> GetTransactionsByCurrency(
        int user_id, std::string_view currency) const;
    utils::ServiceResult<ActivityStats> GetActivitySummary(
        int user_id, dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date) const;

    utils::ServiceResult<std::vector<std::string>> GetDateSamples(
        int user_id, dto::Timeframe timeframe, const DateQueryOptions& options) const;

    utils::ServiceResult<std::set<std::string, std::less<>>> GetAvailableCurrencies(
        int user_id) const;

    utils::ServiceResult<std::vector<WalletTransaction>> GetAllTransactions(int user_id) const;
    utils::ServiceResult<void> AddTransaction(int user_id, std::string_view currency,
                                              std::string_view type, double amount,
                                              utils::TimePoint timestamp) const;

  private:
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    std::shared_ptr<services::UserService> user_service_;
};
} // namespace services