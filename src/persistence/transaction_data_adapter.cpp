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
            record.fields.reserve(5);
            record.fields.push_back(utils::FormatTimestamp(txn.timestamp));
            record.fields.push_back(txn.product_pair);
            record.fields.push_back((txn.type == "Buy") ? "bid" : "ask");
            record.fields.push_back(std::to_string(txn.price));
            record.fields.push_back(std::to_string(txn.amount));
            return record; });
    }
    std::optional<services::Transaction> TransactionDataAdapter::TransformToEntity(
        const data::CsvRecord &record) const
    {
        if (record.fields.size() < 5)
        {
            return std::nullopt;
        }
        services::Transaction txn;
        try
        {
            txn.user_id = std::stoi(record.fields[2]);
        }
        catch (const std::invalid_argument &)
        {
            txn.user_id = 0;
        }
        catch (const std::out_of_range &)
        {
            txn.user_id = 0;
        }
        txn.id = 0;
        txn.product_pair = record.fields[1];
        auto parsed_time = utils::ParseTimestamp(record.fields[0]);
        if (!parsed_time.has_value())
        {
            return std::nullopt;
        }
        txn.timestamp = *parsed_time;
        if (record.fields[2] == "bid")
        {
            txn.type = "Buy";
        }
        else if (record.fields[2] == "ask")
        {
            txn.type = "Sell";
        }
        else
        {
            txn.type = record.fields[2];
        }
        try
        {
            std::string clean_price = record.fields[3];
            std::erase_if(clean_price, [](char c)
                          { return !std::isdigit(c) && c != '.' && c != '-'; });
            std::string clean_amount = record.fields[4];
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
        catch (const std::invalid_argument &)
        {
            return std::nullopt;
        }
        catch (const std::out_of_range &)
        {
            return std::nullopt;
        }
    }
}