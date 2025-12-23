#include "forms/transaction/activity_summary_form_data_provider.hpp"

#include "dto/constants.hpp"

namespace transaction_forms
{

    ActivitySummaryFormDataProvider::ActivitySummaryFormDataProvider(
        std::shared_ptr<services::TradingService> trading_service)
        : trading_service_(std::move(trading_service))
    {
    }

    std::vector<std::string> ActivitySummaryFormDataProvider::GetStartDates(
        std::string_view timeframe) const
    {
        // Request limited dataset from service (lazy loading)
        services::DateQueryOptions options;
        options.limit = 100; // Reasonable UI dropdown limit

        return trading_service_->GetDateSamples(timeframe, options);
    }

    std::vector<std::string> ActivitySummaryFormDataProvider::GetEndDates(
        std::string_view timeframe,
        std::string_view start_date) const
    {
        // Pass filter to service for server-side filtering
        services::DateQueryOptions options;

        if (!start_date.empty())
        {
            options.start_date = start_date; // Server filters: date >= start_date
        }

        options.limit = 100; // Reasonable UI dropdown limit

        // Service applies filter at source - NO client-side filtering!
        return trading_service_->GetDateSamples(timeframe, options);
    }

    std::vector<std::string> ActivitySummaryFormDataProvider::GetDatesByTimeframe(
        std::string_view timeframe) const
    {
        // Legacy method - delegates to GetStartDates
        return GetStartDates(timeframe);
    }

} // namespace transaction_forms
