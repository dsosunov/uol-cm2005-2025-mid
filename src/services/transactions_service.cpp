#include "services/transactions_service.hpp"

#include "core/utils/date_filter.hpp"
#include "dto/constants.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "services/user_service.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

namespace services
{

TransactionsService::TransactionsService(
    std::shared_ptr<persistence::TransactionDataAdapter> adapter,
    std::shared_ptr<services::UserService> user_service)
    : adapter_(std::move(adapter)), user_service_(std::move(user_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("TransactionDataAdapter is required");
    }
    if (!user_service_)
    {
        throw std::invalid_argument("UserService is required");
    }
}

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetLastTransactions(
    int user_id, int count) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::vector<WalletTransaction>>::Failure(validate.message);
    }
    std::vector<WalletTransaction> result;

    adapter_->ReadWithProcessor([&result, user_id, count](const WalletTransaction& transaction) {
        if (transaction.user_id == user_id)
        {
            result.push_back(transaction);
            if (result.size() > static_cast<size_t>(count))
            {
                result.erase(result.begin());
            }
        }
    });

    return {true, "Transactions retrieved successfully", result};
}

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetTransactionsByCurrency(
    int user_id, std::string_view currency) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::vector<WalletTransaction>>::Failure(validate.message);
    }
    std::vector<WalletTransaction> result;

    adapter_->ReadWithProcessor(
        [&result, user_id, &currency](const WalletTransaction& transaction) {
            if (transaction.user_id == user_id && transaction.currency == currency)
            {
                result.push_back(transaction);
            }
        });

    return {true, "Transactions retrieved successfully", result};
}

utils::ServiceResult<ActivityStats> TransactionsService::GetActivitySummary(
    int user_id, [[maybe_unused]] dto::Timeframe timeframe,
    const std::optional<utils::TimePoint>& start_date,
    const std::optional<utils::TimePoint>& end_date) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<ActivityStats>::Failure(validate.message);
    }
    int total = 0;
    double total_volume = 0.0;

    utils::DateFilter date_filter(start_date, end_date);

    adapter_->ReadWithProcessor(
        [&total, &total_volume, user_id, &date_filter](const WalletTransaction& transaction) {
            if (transaction.user_id == user_id && date_filter.IsInRange(transaction.timestamp))
            {
                total++;
                total_volume += transaction.amount;
            }
        });

    double average = (total > 0) ? (total_volume / total) : 0.0;

    return {true, "Activity summary retrieved successfully",
            ActivityStats{total, total_volume, average}};
}

utils::ServiceResult<std::vector<std::string>> TransactionsService::GetDateSamples(
    int user_id, dto::Timeframe timeframe, const DateQueryOptions& options) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::vector<std::string>>::Failure(validate.message);
    }

    std::set<std::string, std::less<>> unique_dates;

    adapter_->ReadWithProcessor(
        [&unique_dates, &options, &timeframe, user_id](const WalletTransaction& transaction) {
            if (transaction.user_id != user_id)
            {
                return;
            }

            if (options.start_date.has_value() && transaction.timestamp < *options.start_date)
            {
                return;
            }

            if (options.end_date.has_value() && transaction.timestamp > *options.end_date)
            {
                return;
            }

            std::string formatted_date = utils::FormatDate(transaction.timestamp);

            if (timeframe == dto::Timeframe::Yearly && formatted_date.length() >= 4)
            {
                unique_dates.insert(formatted_date.substr(0, 4));
            }
            else if (timeframe == dto::Timeframe::Monthly && formatted_date.length() >= 7)
            {
                unique_dates.insert(formatted_date.substr(0, 7));
            }
            else if (formatted_date.length() >= 10)
            {
                unique_dates.insert(formatted_date.substr(0, 10));
            }
        });

    if (unique_dates.empty())
    {
        return {true, "No date samples found", {}};
    }

    std::vector<std::string> result(unique_dates.begin(), unique_dates.end());
    std::vector<std::string> filtered;
    int skipped = 0;

    for (const auto& date : result)
    {
        if (skipped < options.offset)
        {
            skipped++;
            continue;
        }

        filtered.push_back(date);

        if (options.limit.has_value() && filtered.size() >= static_cast<size_t>(*options.limit))
        {
            break;
        }
    }

    return {true, "Date samples retrieved successfully", filtered};
}

utils::ServiceResult<std::set<std::string, std::less<>>> TransactionsService::
    GetAvailableCurrencies(int user_id) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::set<std::string, std::less<>>>::Failure(validate.message);
    }

    std::set<std::string, std::less<>> currencies;
    adapter_->ReadWithProcessor([&currencies, user_id](const WalletTransaction& transaction) {
        if (transaction.user_id == user_id)
        {
            currencies.insert(transaction.currency);
        }
    });

    return {true, "Available currencies retrieved successfully", currencies};
}

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetAllTransactions(
    int user_id) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::vector<WalletTransaction>>::Failure(validate.message);
    }

    std::vector<WalletTransaction> result;
    adapter_->ReadWithProcessor([&result, user_id](const WalletTransaction& transaction) {
        if (transaction.user_id == user_id)
        {
            result.push_back(transaction);
        }
    });

    return utils::ServiceResult<std::vector<WalletTransaction>>::Success(
        result, "Transactions retrieved successfully");
}

utils::ServiceResult<void> TransactionsService::AddTransaction(int user_id,
                                                               std::string_view currency,
                                                               std::string_view type, double amount,
                                                               utils::TimePoint timestamp) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<void>::Failure(validate.message);
    }

    WalletTransaction txn;
    txn.currency = std::string(currency);
    txn.type = std::string(type);
    txn.amount = amount;
    txn.timestamp = timestamp;
    txn.user_id = user_id;

    if (!adapter_->Add(txn))
    {
        return utils::ServiceResult<void>::Failure("Failed to write transaction");
    }

    return utils::ServiceResult<void>::Success("Transaction recorded successfully");
}

} // namespace services