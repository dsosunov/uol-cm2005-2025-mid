#include "persistence/trading_data_adapter.hpp"
#include "services/trading_service.hpp"
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include <algorithm>

namespace persistence
{

    TradingDataAdapter::TradingDataAdapter(std::shared_ptr<data::CsvReader> reader) : reader_(reader)
    {
    }

    void TradingDataAdapter::ReadWithProcessor(
        dto::OrderType order_type,
        std::function<void(const services::OrderRecord &)> processor) const
    {
        if (!reader_ || !reader_->FileExists())
        {
            return;
        }

        // Stream records for efficient processing
        reader_->ReadWithProcessor([&processor, order_type](const data::CsvRecord &csv_record)
                                   {
            auto order = TransformToOrderRecord(csv_record);
            if (order.has_value())
            {
                // Filter by order type using enum comparison
                if (order->order_type == order_type)
                {
                    processor(*order);
                }
            } });
    }

    void TradingDataAdapter::ReadWithProcessor(
        std::function<void(const services::OrderRecord &)> processor) const
    {
        if (!reader_ || !reader_->FileExists())
        {
            return;
        }

        // Stream all records without filtering
        reader_->ReadWithProcessor([&processor](const data::CsvRecord &csv_record)
                                   {
            auto order = TransformToOrderRecord(csv_record);
            if (order.has_value())
            {
                processor(*order);
            } });
    }

    bool TradingDataAdapter::WriteAll(data::CsvWriter &writer,
                                      const std::vector<services::OrderRecord> &records)
    {
        // Pure transformation - write all data
        for (const auto &order : records)
        {
            data::CsvRecord record = TransformFromOrderRecord(order);
            if (!writer.Write(record))
            {
                return false;
            }
        }
        return writer.Flush();
    }

    std::optional<services::OrderRecord> TradingDataAdapter::TransformToOrderRecord(
        const data::CsvRecord &record)
    {
        services::OrderRecord order;

        order.product_pair = record.product;

        // Convert CSV string to enum ("ask" -> Asks, "bid" -> Bids)
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
            return std::nullopt; // Invalid order type
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
            clean_price.erase(
                std::remove_if(clean_price.begin(), clean_price.end(),
                               [](char c)
                               { return !std::isdigit(c) && c != '.' && c != '-'; }),
                clean_price.end());

            std::string clean_amount = record.amount;
            clean_amount.erase(
                std::remove_if(clean_amount.begin(), clean_amount.end(),
                               [](char c)
                               { return !std::isdigit(c) && c != '.' && c != '-'; }),
                clean_amount.end());

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

        // Convert enum to CSV string (Asks -> "ask", Bids -> "bid")
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
