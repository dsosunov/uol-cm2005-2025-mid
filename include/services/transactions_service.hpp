#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace services
{

    struct Transaction
    {
        int id;
        std::string product_pair;
        std::string type; // "Buy" or "Sell"
        double amount;
        double price;
        std::string timestamp;
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
        TransactionsService();
        ~TransactionsService() = default;

        // Query operations
        std::vector<Transaction> GetLastTransactions(int count,
                                                     std::optional<int> user_id = std::nullopt) const;
        std::vector<Transaction> GetTransactionsByPair(std::string_view product_pair,
                                                       std::optional<int> user_id = std::nullopt) const;
        ActivityStats GetActivitySummary(std::string_view timeframe, std::string_view start_date,
                                         std::string_view end_date,
                                         std::optional<int> user_id = std::nullopt) const;

        // Mutation operations
        bool AddTransaction(const Transaction &transaction);

    private:
        std::vector<Transaction> transactions_;
        int next_transaction_id_ = 6;
        int default_user_id_ = 1;

        // Helper to get effective user ID
        int GetEffectiveUserId(std::optional<int> user_id) const;
    };

} // namespace services
