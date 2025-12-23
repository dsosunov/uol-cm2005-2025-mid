#include "persistence/transaction_data_adapter.hpp"
#include "services/transactions_service.hpp"
#include "core/utils/time_utils.hpp"
#include <algorithm>
namespace persistence
{
    TransactionDataAdapter::TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader)
        : BaseDataAdapter<services::Transaction>(reader)
    {
    }
    bool TransactionDataAdapter::WriteAll(
        data::CsvWriter &writer, const std::vector<services::Transaction> &transactions)
    {
        return BaseDataAdapter<services::Transaction>::WriteAll(writer, transactions,
                                                                [](const services::Transaction &txn)
                                                                {
            data::CsvRecord record;
            record.timestamp = utils::FormatTimestamp(txn.timestamp);
            record.product = txn.product_pair;
            record.order_type = (txn.type == "Buy") ? "bid" : "ask";
            record.price = std::to_string(txn.price);
            record.amount = std::to_string(txn.amount);
            return record; });
    }
    std::optional<services::Transaction> TransactionDataAdapter::TransformToEntity(
        const data::CsvRecord &record) const
    {
        services::Transaction txn;
        try
        {
            txn.user_id = std::stoi(record.order_type);
        }
        catch (...)
        {
            txn.user_id = 0;
        }
        txn.id = 0;
        txn.product_pair = record.product;
        auto parsed_time = utils::ParseTimestamp(record.timestamp);
        if (!parsed_time.has_value())
        {
            return std::nullopt;
        }
        txn.timestamp = *parsed_time;
        if (record.order_type == "bid")
        {
            txn.type = "Buy";
        }
        else if (record.order_type == "ask")
        {
            txn.type = "Sell";
        }
        else
        {
            txn.type = record.order_type;
        }
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
            txn.price = std::stod(clean_price);
            txn.amount = std::stod(clean_amount);
            return txn;
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
}