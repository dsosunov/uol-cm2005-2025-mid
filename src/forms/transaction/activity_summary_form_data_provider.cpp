#include "forms/transaction/activity_summary_form_data_provider.hpp"

#include "dto/constants.hpp"

namespace transaction_forms
{
ActivitySummaryFormDataProvider::ActivitySummaryFormDataProvider(
    std::shared_ptr<services::TransactionsService> transactions_service, int user_id)
    : transactions_service_(std::move(transactions_service)), user_id_(user_id)
{
}

std::vector<ActivitySummaryFormDataProvider::OptionPair> ActivitySummaryFormDataProvider::
    GetStartDates(dto::Timeframe timeframe) const
{
    services::DateQueryOptions options;
    options.limit = 100;

    auto date_result = transactions_service_->GetDateSamples(user_id_, timeframe, options);
    if (!date_result.success || !date_result.data.has_value())
    {
        return {};
    }

    const auto& dates = *date_result.data;

    std::vector<OptionPair> pairs;
    pairs.reserve(dates.size());

    for (const auto& date : dates)
    {
        pairs.emplace_back(date, date);
    }

    return pairs;
}

std::vector<ActivitySummaryFormDataProvider::OptionPair> ActivitySummaryFormDataProvider::
    GetEndDates(dto::Timeframe timeframe, std::optional<utils::TimePoint> start_date) const
{
    services::DateQueryOptions options;

    if (start_date.has_value())
    {
        options.start_date = *start_date;
    }

    options.limit = 100;

    auto date_result = transactions_service_->GetDateSamples(user_id_, timeframe, options);
    if (!date_result.success || !date_result.data.has_value())
    {
        return {};
    }

    const auto& dates = *date_result.data;

    std::vector<OptionPair> pairs;
    pairs.reserve(dates.size());

    for (const auto& date : dates)
    {
        pairs.emplace_back(date, date);
    }

    return pairs;
}

} // namespace transaction_forms