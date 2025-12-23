#include "services/transactions_service.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "dto/constants.hpp"
#include "core/utils/service_utils.hpp"
#include <algorithm>
namespace services
{
    TransactionsService::TransactionsService(
        std::shared_ptr<persistence::TransactionDataAdapter> adapter)
        : adapter_(adapter)
    {
    }
    int TransactionsService::GetEffectiveUserId(std::optional<int> user_id) const
    {
        return ServiceUtils::GetEffectiveUserId(user_id, default_user_id_);
    }
    std::vector<Transaction> TransactionsService::GetLastTransactions(
        int count, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        std::vector<Transaction> result;
        if (adapter_)
        {
            adapter_->ReadWithProcessor([&result, effective_id, count](const Transaction &transaction)
                                        {
                if (transaction.user_id == effective_id && result.size() < static_cast<size_t>(count))
                {
                    result.push_back(transaction);
                } });
        }
        else
        {
            for (auto it = transactions_.rbegin();
                 it != transactions_.rend() && result.size() < static_cast<size_t>(count); ++it)
            {
                if (it->user_id == effective_id)
                {
                    result.push_back(*it);
                }
            }
        }
        return result;
    }
    std::vector<Transaction> TransactionsService::GetTransactionsByPair(
        std::string_view product_pair, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        std::vector<Transaction> result;
        if (adapter_)
        {
            adapter_->ReadWithProcessor([&result, effective_id, &product_pair](const Transaction &transaction)
                                        {
                if (transaction.user_id == effective_id && transaction.product_pair == product_pair)
                {
                    result.push_back(transaction);
                } });
        }
        else
        {
            for (const auto &transaction : transactions_)
            {
                if (transaction.user_id == effective_id && transaction.product_pair == product_pair)
                {
                    result.push_back(transaction);
                }
            }
        }
        return result;
    }
    ActivityStats TransactionsService::GetActivitySummary([[maybe_unused]] dto::Timeframe timeframe,
                                                          [[maybe_unused]] const std::optional<utils::TimePoint> &start_date,
                                                          [[maybe_unused]] const std::optional<utils::TimePoint> &end_date,
                                                          std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
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
}