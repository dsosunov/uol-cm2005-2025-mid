#include "persistence/trading_data_adapter.hpp"
#include "services/trading_service.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include <algorithm>
namespace persistence
{
    TradingDataAdapter::TradingDataAdapter(std::shared_ptr<data::CsvReader> reader)
        : BaseDataAdapter<services::OrderRecord>(reader)
    {
    }
    void TradingDataAdapter::ReadWithProcessor(
        dto::OrderType order_type,
        const std::function<void(const services::OrderRecord &)> &processor) const
    {
        BaseDataAdapter<services::OrderRecord>::ReadWithProcessor(
            [order_type](const services::OrderRecord &order)
            { return order.order_type == order_type; },
            processor);
    }
    void TradingDataAdapter::ReadWithProcessor(
        const std::function<void(const services::OrderRecord &)> &processor) const
    {
        BaseDataAdapter<services::OrderRecord>::ReadWithProcessor(
            [](const services::OrderRecord &)
            { return true; },
            processor);
    }
    bool TradingDataAdapter::WriteAll(data::CsvWriter &writer,
                                      const std::vector<services::OrderRecord> &records)
    {
        return BaseDataAdapter<services::OrderRecord>::WriteAll(writer, records, TransformFromOrderRecord);
    }
    std::optional<services::OrderRecord> TradingDataAdapter::TransformToEntity(
        const data::CsvRecord &record) const
    {
        services::OrderRecord order;
        order.product_pair = record.product;
        if (record.order_type == "ask")
        {
            order.order_type = dto::OrderType::Asks;
        }
        else if (record.order_type == "bid")
        {
            order.order_type = dto::OrderType::Bids;
        }
        else
        {
            return std::nullopt;
        }
        auto parsed_time = utils::ParseTimestamp(record.timestamp);
        if (!parsed_time.has_value())
        {
            return std::nullopt;
        }
        order.timestamp = *parsed_time;
        try
        {
            std::string clean_price = record.price;
            std::erase_if(clean_price, [](char c)
                          { return !std::isdigit(c) && c != '.' && c != '-'; });
            std::string clean_amount = record.amount;
            std::erase_if(clean_amount, [](char c)
                          { return !std::isdigit(c) && c != '.' && c != '-'; });
            if (clean_price.empty() || clean_amount.empty())
            {
                return std::nullopt;
            }
            order.price = std::stod(clean_price);
            order.amount = std::stod(clean_amount);
            return order;
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
    data::CsvRecord TradingDataAdapter::TransformFromOrderRecord(
        const services::OrderRecord &order)
    {
        data::CsvRecord record;
        record.timestamp = utils::FormatTimestamp(order.timestamp);
        record.product = order.product_pair;
        std::string order_type_str{dto::OrderTypeToString(order.order_type)};
        if (!order_type_str.empty() && order_type_str.back() == 's')
        {
            order_type_str.pop_back();
        }
        record.order_type = order_type_str;
        record.price = std::to_string(order.price);
        record.amount = std::to_string(order.amount);
        return record;
    }
}