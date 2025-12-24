#include "persistence/trading_data_adapter.hpp"

#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/trading_service.hpp"

#include <algorithm>

namespace persistence
{

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

    try
    {
        std::string clean_price = CleanNumericField(record.fields[3]);
        std::string clean_amount = CleanNumericField(record.fields[4]);

        if (clean_price.empty() || clean_amount.empty())
        {
            return std::nullopt;
        }

        order.price = std::stod(clean_price);
        order.amount = std::stod(clean_amount);

        return order;
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }
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

std::string TradingDataAdapter::CleanNumericField(const std::string& field)
{
    std::string cleaned = field;
    std::erase_if(cleaned, [](char c) { return !std::isdigit(c) && c != '.' && c != '-'; });
    return cleaned;
}

} // namespace persistence