#include "persistence/transaction_data_adapter.hpp"
#include "services/transactions_service.hpp"
#include "core/utils/time_utils.hpp"
#include <algorithm>

namespace persistence
{

    TransactionDataAdapter::TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader)
        : reader_(reader)
    {
    }

    void TransactionDataAdapter::ReadWithProcessor(
        std::function<void(const services::Transaction &)> processor) const
    {
        if (!reader_ || !reader_->FileExists())
        {
            return;
        }

        // Stream records for efficient processing
        reader_->ReadWithProcessor([&processor](const data::CsvRecord &csv_record)
                                   {
            auto transaction = TransformToTransaction(csv_record);
            if (transaction.has_value())
            {
                processor(*transaction);
            } });
    }

    bool TransactionDataAdapter::WriteAll(
        data::CsvWriter &writer, const std::vector<services::Transaction> &transactions)
    {
        for (const auto &txn : transactions)
        {
            data::CsvRecord record;
            record.timestamp = utils::FormatTimestamp(txn.timestamp);
            record.product = txn.product_pair;
            record.order_type = (txn.type == "Buy") ? "bid" : "ask";
            record.price = std::to_string(txn.price);
            record.amount = std::to_string(txn.amount);

            if (!writer.Write(record))
            {
                return false;
            }
        }
        return writer.Flush();
    }

    std::optional<services::Transaction> TransactionDataAdapter::TransformToTransaction(
        const data::CsvRecord &record)
    {
        services::Transaction txn;

        // Extract user_id from order_type field (this is how it's stored in CSV)
        try
        {
            txn.user_id = std::stoi(record.order_type);
        }
        catch (...)
        {
            // If not a user_id, parse as order type
            txn.user_id = 0;
        }

        txn.id = 0; // Service will assign IDs
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
