#include "services/transactions_service.hpp"

#include "core/utils/date_filter.hpp"
#include "dto/constants.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "services/authentication_service.hpp"

#include <algorithm>
#include <set>
#include <stdexcept>

namespace services
{

TransactionsService::TransactionsService(
    std::shared_ptr<persistence::TransactionDataAdapter> adapter,
    std::shared_ptr<services::AuthenticationService> auth_service)
    : adapter_(std::move(adapter)), auth_service_(std::move(auth_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("TransactionDataAdapter is required");
    }
    if (!auth_service_)
    {
        throw std::invalid_argument("AuthenticationService is required");
    }
}

utils::ServiceResult<std::vector<WalletTransaction>> TransactionsService::GetLastTransactions(
    int count) const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<std::vector<WalletTransaction>>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;
    std::vector<WalletTransaction> result;

    adapter_->ReadWithProcessor(
        [&result, effective_id, count](const WalletTransaction& transaction) {
            if (transaction.user_id == effective_id)
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
    std::string_view currency) const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<std::vector<WalletTransaction>>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;
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
    const std::optional<utils::TimePoint>& end_date) const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<ActivityStats>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;
    int total = 0;
    double total_volume = 0.0;

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

utils::ServiceResult<std::vector<std::string>> TransactionsService::GetDateSamples(
    dto::Timeframe timeframe, const DateQueryOptions& options) const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<std::vector<std::string>>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;

    std::set<std::string, std::less<>> unique_dates;

    adapter_->ReadWithProcessor(
        [&unique_dates, &options, &timeframe, effective_id](const WalletTransaction& transaction) {
            if (transaction.user_id != effective_id)
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
    GetAvailableCurrencies() const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<std::set<std::string, std::less<>>>::Failure(
            user_result.message);
    }

    int effective_id = user_result.data.value().id;

    std::set<std::string, std::less<>> currencies;
    adapter_->ReadWithProcessor([&currencies, effective_id](const WalletTransaction& transaction) {
        if (transaction.user_id == effective_id)
        {
            currencies.insert(transaction.currency);
        }
    });

    return {true, "Available currencies retrieved successfully", currencies};
}

} // namespace services