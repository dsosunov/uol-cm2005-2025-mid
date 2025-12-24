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
    if (auto result = trading_service_->GetAvailableProducts();
        result.success && result.data.has_value())
    {
        return *result.data;
    }
    return {};
}

std::vector<CandlestickFormDataProvider::OptionPair> CandlestickFormDataProvider::GetStartDates(
    dto::Timeframe timeframe) const
{
    services::DateQueryOptions options;
    options.limit = 100;

    auto date_result = trading_service_->GetDateSamples(timeframe, options);
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
    dto::Timeframe timeframe, std::optional<utils::TimePoint> start_date) const
{
    services::DateQueryOptions options;
    if (start_date.has_value())
    {
        options.start_date = *start_date;
    }

    options.limit = 100;

    auto date_result = trading_service_->GetDateSamples(timeframe, options);
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

} // namespace candlestick