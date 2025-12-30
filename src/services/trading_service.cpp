#include "services/trading_service.hpp"

#include "app_constants.hpp"
#include "dto/constants.hpp"
#include "persistence/trading_data_adapter.hpp"

#include <functional>

namespace services
{
TradingService::TradingService(std::shared_ptr<persistence::TradingDataAdapter> adapter)
    : adapter_(adapter)
{
}

utils::ServiceResult<void> TradingService::ForEachOrder(
    dto::OrderType order_type, const std::function<void(const OrderRecord&)>& processor) const
{
    if (!adapter_)
    {
        return utils::ServiceResult<void>::Failure("Trading data adapter is not configured");
    }

    adapter_->ReadWithProcessor(order_type, processor);
    return utils::ServiceResult<void>::Success("Orders streamed successfully");
}

utils::ServiceResult<std::set<std::string, std::less<>>> TradingService::GetAvailableProducts()
    const
{
    // Supported currencies are defined at the app level to keep behavior stable even when the
    // system starts with no market data.
    return {true, "Available products retrieved successfully", app::kSupportedCurrencies};
}

utils::ServiceResult<std::vector<std::string>> TradingService::GetDateSamples(
    dto::Timeframe timeframe, const DateQueryOptions& options) const
{
    std::set<std::string, std::less<>> unique_dates;

    adapter_->ReadWithProcessor([&unique_dates, &options, &timeframe](const OrderRecord& order) {
        if (options.start_date.has_value() && order.timestamp < *options.start_date)
            return;

        if (options.end_date.has_value() && order.timestamp > *options.end_date)
            return;

        std::string formatted_date = utils::FormatDate(order.timestamp);

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
            break;
    }

    return {true, "Date samples retrieved successfully", filtered};
}

utils::ServiceResult<void> TradingService::AppendOrders(
    const std::vector<OrderRecord>& orders) const
{
    if (orders.empty())
    {
        return utils::ServiceResult<void>::Success("No orders to append");
    }

    if (!adapter_->AddAll(orders))
    {
        return utils::ServiceResult<void>::Failure("Failed to append orders");
    }

    return utils::ServiceResult<void>::Success("Orders appended successfully");
}

} // namespace services