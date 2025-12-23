#include "services/transactions_service.hpp"
#include <algorithm>

namespace services
{

    TransactionsService::TransactionsService()
        : transactions_({{1, "USD/EUR", "Buy", 100.00, 0.95, "2025-12-22 10:30", 1},
                         {2, "GBP/USD", "Sell", 50.00, 1.28, "2025-12-22 09:15", 1},
                         {3, "USD/JPY", "Buy", 200.00, 150.5, "2025-12-21 15:45", 1},
                         {4, "EUR/GBP", "Sell", 75.00, 0.85, "2025-12-21 12:20", 1},
                         {5, "CAD/USD", "Buy", 150.00, 0.73, "2025-12-20 16:00", 1}})
    {
    }

    int TransactionsService::GetEffectiveUserId(std::optional<int> user_id) const
    {
        return user_id.value_or(default_user_id_);
    }

    std::vector<Transaction> TransactionsService::GetLastTransactions(
        int count, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        std::vector<Transaction> result;

        // Filter by user and get last N transactions
        for (auto it = transactions_.rbegin(); it != transactions_.rend() && result.size() < count;
             ++it)
        {
            if (it->user_id == effective_id)
            {
                result.push_back(*it);
            }
        }

        return result;
    }

    std::vector<Transaction> TransactionsService::GetTransactionsByPair(
        std::string_view product_pair, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        std::vector<Transaction> result;

        for (const auto &transaction : transactions_)
        {
            if (transaction.user_id == effective_id && transaction.product_pair == product_pair)
            {
                result.push_back(transaction);
            }
        }

        return result;
    }

    ActivityStats TransactionsService::GetActivitySummary([[maybe_unused]] std::string_view timeframe,
                                                          [[maybe_unused]] std::string_view start_date,
                                                          [[maybe_unused]] std::string_view end_date,
                                                          std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);

        // For simplicity, we'll just count all transactions and calculate stats
        // In a real system, we'd filter by date range
        int total = 0;
        double total_volume = 0.0;

        for (const auto &transaction : transactions_)
        {
            if (transaction.user_id == effective_id)
            {
                total++;
                total_volume += transaction.amount * transaction.price;
            }
        }

        double average = (total > 0) ? (total_volume / total) : 0.0;

        return {total, total_volume, average};
    }

    bool TransactionsService::AddTransaction(const Transaction &transaction)
    {
        Transaction new_transaction = transaction;
        new_transaction.id = next_transaction_id_++;
        transactions_.push_back(new_transaction);
        return true;
    }

} // namespace services
