#include "forms/transaction/activity_summary_form_data_provider.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
namespace transaction_forms
{
    ActivitySummaryFormDataProvider::ActivitySummaryFormDataProvider(
        std::shared_ptr<services::TradingService> trading_service)
        : trading_service_(std::move(trading_service))
    {
    }
    std::vector<ActivitySummaryFormDataProvider::OptionPair> ActivitySummaryFormDataProvider::GetStartDates(
        std::string_view timeframe) const
    {
        services::DateQueryOptions options;
        options.limit = 100;
        dto::Timeframe timeframe_enum = dto::Timeframe::Daily;
        if (timeframe == "monthly")
            timeframe_enum = dto::Timeframe::Monthly;
        else if (timeframe == "yearly")
            timeframe_enum = dto::Timeframe::Yearly;
        auto dates = trading_service_->GetDateSamples(timeframe_enum, options);
        std::vector<OptionPair> pairs;
        pairs.reserve(dates.size());
        for (const auto &date : dates)
        {
            pairs.emplace_back(date, date);
        }
        return pairs;
    }
    std::vector<ActivitySummaryFormDataProvider::OptionPair> ActivitySummaryFormDataProvider::GetEndDates(
        std::string_view timeframe,
        std::string_view start_date) const
    {
        services::DateQueryOptions options;
        if (!start_date.empty())
        {
            auto parsed = utils::ParseTimestamp(start_date);
            if (parsed.has_value())
            {
                options.start_date = *parsed;
            }
        }
        options.limit = 100;
        dto::Timeframe timeframe_enum = dto::Timeframe::Daily;
        if (timeframe == "monthly")
            timeframe_enum = dto::Timeframe::Monthly;
        else if (timeframe == "yearly")
            timeframe_enum = dto::Timeframe::Yearly;
        auto dates = trading_service_->GetDateSamples(timeframe_enum, options);
        std::vector<OptionPair> pairs;
        pairs.reserve(dates.size());
        for (const auto &date : dates)
        {
            pairs.emplace_back(date, date);
        }
        return pairs;
    }
    std::vector<ActivitySummaryFormDataProvider::OptionPair> ActivitySummaryFormDataProvider::GetDatesByTimeframe(
        std::string_view timeframe) const
    {
        return GetStartDates(timeframe);
    }
}