#pragma once
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace persistence
{
class TransactionDataAdapter;
}
namespace services
{
struct WalletTransaction
{
    std::string currency;
    std::string type; // "Deposit" or "Withdraw"
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
    TransactionsService() = default;
    explicit TransactionsService(std::shared_ptr<persistence::TransactionDataAdapter> adapter);
    ~TransactionsService() = default;
    utils::ServiceResult<std::vector<WalletTransaction>> GetLastTransactions(
        int count, std::optional<int> user_id = std::nullopt) const;
    utils::ServiceResult<std::vector<WalletTransaction>> GetTransactionsByCurrency(
        std::string_view currency, std::optional<int> user_id = std::nullopt) const;
    utils::ServiceResult<ActivityStats> GetActivitySummary(
        dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date,
        std::optional<int> user_id = std::nullopt) const;

  private:
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    int default_user_id_ = 1;
    int GetEffectiveUserId(std::optional<int> user_id) const;
};
} // namespace services