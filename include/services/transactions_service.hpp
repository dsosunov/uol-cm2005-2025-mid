#pragma once
#include <optional>
#include <string>
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
        std::vector<Transaction> GetTransactionsByPair(const std::string &product_pair,
                                                       std::optional<int> user_id = std::nullopt) const;
        ActivityStats GetActivitySummary(const std::string &timeframe, const std::string &start_date,
                                         const std::string &end_date,
                                         std::optional<int> user_id = std::nullopt) const;

        // Mutation operations
        bool AddTransaction(const Transaction &transaction);

    private:
        std::vector<Transaction> transactions_;
        int next_transaction_id_;
        int default_user_id_;

        // Helper to get effective user ID
        int GetEffectiveUserId(std::optional<int> user_id) const;
    };

} // namespace services
