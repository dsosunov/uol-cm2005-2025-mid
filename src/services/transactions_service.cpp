#include "services/transactions_service.hpp"

#include "core/utils/date_filter.hpp"
#include "core/utils/service_utils.hpp"
#include "dto/constants.hpp"
#include "persistence/transaction_data_adapter.hpp"

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

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetLastTransactions(
    int count, std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    std::vector<WalletTransaction> result;
    result.reserve(count);

    // Read from end of file backwards until we collect enough transactions for this user
    adapter_->ReadReverseWithProcessor(
        [&result, effective_id, count](const WalletTransaction& transaction) {
            if (transaction.user_id == effective_id && result.size() < static_cast<size_t>(count))
            {
                result.push_back(transaction);
            }
        });

    return {true, "Transactions retrieved successfully", result};
}

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetTransactionsByCurrency(
    std::string_view currency, std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    std::vector<WalletTransaction> result;

    adapter_->ReadWithProcessor(
        [&result, effective_id, &currency](const WalletTransaction& transaction) {
            if (transaction.user_id == effective_id && transaction.currency == currency)
            {
                result.push_back(transaction);
            }
        });

    return {true, "Transactions retrieved successfully", result};
}

utils::ServiceResult<ActivityStats> TransactionsService::GetActivitySummary(
    [[maybe_unused]] dto::Timeframe timeframe, const std::optional<utils::TimePoint>& start_date,
    const std::optional<utils::TimePoint>& end_date, std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    int total = 0;
    double total_volume = 0.0;

    // Create date filter
    utils::DateFilter date_filter(start_date, end_date);

    adapter_->ReadWithProcessor(
        [&total, &total_volume, effective_id, &date_filter](const WalletTransaction& transaction) {
            if (transaction.user_id == effective_id && date_filter.IsInRange(transaction.timestamp))
            {
                total++;
                total_volume += transaction.amount;
            }
        });

    double average = (total > 0) ? (total_volume / total) : 0.0;

    return {true, "Activity summary retrieved successfully",
            ActivityStats{total, total_volume, average}};
}

} // namespace services