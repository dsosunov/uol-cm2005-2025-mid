#pragma once
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
struct Transaction
{
    int id;
    std::string product_pair;
    std::string type;
    double amount;
    double price;
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
    std::vector<Transaction> GetLastTransactions(int count,
                                                 std::optional<int> user_id = std::nullopt) const;
    std::vector<Transaction> GetTransactionsByPair(std::string_view product_pair,
                                                   std::optional<int> user_id = std::nullopt) const;
    ActivityStats GetActivitySummary(dto::Timeframe timeframe,
                                     const std::optional<utils::TimePoint>& start_date,
                                     const std::optional<utils::TimePoint>& end_date,
                                     std::optional<int> user_id = std::nullopt) const;
    bool AddTransaction(const Transaction& transaction);

  private:
    std::vector<Transaction> transactions_ = {
        {1, "USD/EUR", "Buy", 100.00, 0.95, utils::ParseTimestamp("2025-12-22 10:30").value(), 1},
        {2, "GBP/USD", "Sell", 50.00, 1.28, utils::ParseTimestamp("2025-12-22 09:15").value(), 1},
        {3, "USD/JPY", "Buy", 200.00, 150.5, utils::ParseTimestamp("2025-12-21 15:45").value(), 1},
        {4, "EUR/GBP", "Sell", 75.00, 0.85, utils::ParseTimestamp("2025-12-21 12:20").value(), 1},
        {5, "CAD/USD", "Buy", 150.00, 0.73, utils::ParseTimestamp("2025-12-20 16:00").value(), 1}};
    int next_transaction_id_ = 6;
    int default_user_id_ = 1;
    std::shared_ptr<persistence::TransactionDataAdapter> adapter_;
    int GetEffectiveUserId(std::optional<int> user_id) const;
};
} // namespace services