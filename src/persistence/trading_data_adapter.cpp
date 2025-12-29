#include "persistence/trading_data_adapter.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/trading_service.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <string_view>

namespace persistence
{

std::string_view TradingDataAdapter::TrimWhitespace(std::string_view text)
{
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
    {
        text.remove_prefix(1);
    }

    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
    {
        text.remove_suffix(1);
    }

    return text;
}

std::optional<double> TradingDataAdapter::ParseStrictDouble(std::string_view text)
{
    text = TrimWhitespace(text);
    if (text.empty())
    {
        return std::nullopt;
    }

    // Fast, allocation-free parse. from_chars does not skip whitespace, so we trimmed above.
    double value = 0.0;
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    auto [ptr, ec] = std::from_chars(begin, end, value, std::chars_format::general);
    if (ec != std::errc{})
    {
        return std::nullopt;
    }

    std::string_view remainder{ptr, static_cast<std::size_t>(end - ptr)};
    remainder = TrimWhitespace(remainder);
    if (!remainder.empty())
    {
        // Reject values like "0.021873a78" instead of mutating them.
        return std::nullopt;
    }

    if (!std::isfinite(value))
    {
        return std::nullopt;
    }

    return value;
}

TradingDataAdapter::TradingDataAdapter(std::shared_ptr<data::CsvReader> reader)
    : BaseDataAdapter<services::OrderRecord>(reader)
{
}

void TradingDataAdapter::ReadWithProcessor(
    dto::OrderType order_type,
    const std::function<void(const services::OrderRecord&)>& processor) const
{
    BaseDataAdapter<services::OrderRecord>::ReadWithProcessor(
        [order_type](const services::OrderRecord& order) { return order.order_type == order_type; },
        processor);
}

void TradingDataAdapter::ReadWithProcessor(
    const std::function<void(const services::OrderRecord&)>& processor) const
{
    BaseDataAdapter<services::OrderRecord>::ReadWithProcessor(
        [](const services::OrderRecord&) { return true; }, processor);
}

bool TradingDataAdapter::WriteAll(data::CsvWriter& writer,
                                  const std::vector<services::OrderRecord>& records)
{
    return BaseDataAdapter<services::OrderRecord>::WriteAll(writer, records,
                                                            TransformFromOrderRecord);
}

std::optional<services::OrderRecord> TradingDataAdapter::TransformToEntity(
    const data::CsvRecord& record) const
{
    if (record.fields.size() < 5)
    {
        return std::nullopt;
    }

    services::OrderRecord order;
    order.product_pair = record.fields[1];
    if (record.fields[2] == "ask")
    {
        order.order_type = dto::OrderType::Asks;
    }
    else if (record.fields[2] == "bid")
    {
        order.order_type = dto::OrderType::Bids;
    }
    else
    {
        return std::nullopt;
    }

    auto parsed_time = utils::ParseTimestamp(record.fields[0]);
    if (!parsed_time.has_value())
    {
        return std::nullopt;
    }

    order.timestamp = *parsed_time;

    auto parsed_price = ParseStrictDouble(record.fields[3]);
    auto parsed_amount = ParseStrictDouble(record.fields[4]);
    if (!parsed_price.has_value() || !parsed_amount.has_value())
    {
        return std::nullopt;
    }

    order.price = *parsed_price;
    order.amount = *parsed_amount;

    return order;
}

data::CsvRecord TradingDataAdapter::TransformFromOrderRecord(const services::OrderRecord& order)
{
    data::CsvRecord record;
    record.fields.reserve(5);

    record.fields.push_back(utils::FormatTimestamp(order.timestamp));
    record.fields.push_back(order.product_pair);

    std::string order_type_str{dto::OrderTypeToString(order.order_type)};

    if (!order_type_str.empty() && order_type_str.back() == 's')
    {
        order_type_str.pop_back();
    }

    record.fields.push_back(order_type_str);
    record.fields.push_back(std::to_string(order.price));
    record.fields.push_back(std::to_string(order.amount));

    return record;
}

} // namespace persistence