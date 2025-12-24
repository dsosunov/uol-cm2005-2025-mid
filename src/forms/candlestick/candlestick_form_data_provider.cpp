#include "forms/candlestick/candlestick_form_data_provider.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"

namespace candlestick
{
CandlestickFormDataProvider::CandlestickFormDataProvider(
    std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

std::set<std::string, std::less<>> CandlestickFormDataProvider::GetAvailableProducts() const
{
    auto result = trading_service_->GetAvailableProducts();
    if (result.success && result.data.has_value())
    {
        return *result.data;
    }
    return {};
}
std::vector<CandlestickFormDataProvider::OptionPair> CandlestickFormDataProvider::GetStartDates(
    std::string_view timeframe) const
{
    using enum dto::Timeframe;
    services::DateQueryOptions options;
    options.limit = 100;

    dto::Timeframe timeframe_enum = Daily;
    if (timeframe == "monthly")
        timeframe_enum = Monthly;
    else if (timeframe == "yearly")
        timeframe_enum = Yearly;

    auto date_result = trading_service_->GetDateSamples(timeframe_enum, options);
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
std::vector<CandlestickFormDataProvider::OptionPair> CandlestickFormDataProvider::GetEndDates(
    std::string_view timeframe, std::string_view start_date) const
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
    auto date_result = trading_service_->GetDateSamples(timeframe_enum, options);
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
std::vector<CandlestickFormDataProvider::OptionPair> CandlestickFormDataProvider::
    GetDatesByTimeframe(std::string_view timeframe) const
{
    return GetStartDates(timeframe);
}
} // namespace candlestick