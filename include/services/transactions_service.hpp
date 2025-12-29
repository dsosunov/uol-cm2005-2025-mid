#pragma once
#include "core/utils/service_result.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/date_query_options.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace services
{
class AuthenticationService;
}

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
    TransactionsService(std::shared_ptr<persistence::TransactionDataAdapter> adapter,
                        std::shared_ptr<services::AuthenticationService> auth_service);
    ~TransactionsService() = default;
    utils::ServiceResult<std::vector<WalletTransaction>> GetLastTransactions(int count) const;
    utils::ServiceResult<std::vector<WalletTransaction>> GetTransactionsByCurrency(
        std::string_view currency) const;
    utils::ServiceResult<ActivityStats> GetActivitySummary(
        dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
        const std::optional<utils::TimePoint>& end_date) const;

    utils::ServiceResult<std::vector<std::string>> GetDateSamples(
        dto::Timeframe timeframe, const DateQueryOptions& options) const;

  private:
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
};
} // namespace services