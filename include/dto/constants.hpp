#pragma once
#include <string>
#include <string_view>
namespace dto
{
enum class Timeframe
{
    Daily,
    Monthly,
    Yearly
};
enum class OrderType
{
    Asks,
    Bids
};
inline std::string_view TimeframeToString(Timeframe timeframe)
{
    using enum Timeframe;
    switch (timeframe)
    {
    case Daily:
        return "daily";
    case Monthly:
        return "monthly";
    case Yearly:
        return "yearly";
    default:
        return "";
    }
}
inline std::string_view OrderTypeToString(OrderType order_type)
{
    switch (order_type)
    {
    case OrderType::Asks:
        return "asks";
    case OrderType::Bids:
        return "bids";
    default:
        return "";
    }
}
} // namespace dto