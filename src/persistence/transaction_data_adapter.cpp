#include "persistence/transaction_data_adapter.hpp"

#include "core/data/csv_writer.hpp"
#include "core/utils/time_utils.hpp"
#include "services/transactions_service.hpp"

#include <algorithm>

namespace persistence
{

TransactionDataAdapter::TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader,
                                               std::shared_ptr<data::CsvWriter> writer)
    : BaseDataAdapter<services::WalletTransaction>(reader), writer_(writer)
{
}

data::CsvRecord TransactionDataAdapter::TransformFromTransaction(
    const services::WalletTransaction& txn)
{
    data::CsvRecord record;
    record.fields.reserve(5);

    record.fields.emplace_back(utils::FormatTimestamp(txn.timestamp));
    record.fields.emplace_back(txn.currency);
    record.fields.emplace_back(txn.type);
    record.fields.emplace_back(std::to_string(txn.amount));
    record.fields.emplace_back(std::to_string(txn.user_id));

    return record;
}

std::string TransactionDataAdapter::CleanNumericField(const std::string& field)
{
    std::string cleaned = field;
    std::erase_if(cleaned, [](char c) { return !std::isdigit(c) && c != '.' && c != '-'; });
    return cleaned;
}

bool TransactionDataAdapter::Add(const services::WalletTransaction& transaction)
{
    if (!writer_)
    {
        return false;
    }

    auto record = TransformFromTransaction(transaction);
    return writer_->Write(record) && writer_->Flush();
}

std::optional<services::WalletTransaction> TransactionDataAdapter::TransformToEntity(
    const data::CsvRecord& record) const
{
    if (record.fields.size() < 5)
    {
        return std::nullopt;
    }

    services::WalletTransaction txn;

    // Parse timestamp
    auto parsed_time = utils::ParseTimestamp(record.fields[0]);
    if (!parsed_time.has_value())
    {
        return std::nullopt;
    }
    txn.timestamp = *parsed_time;

    // Currency
    txn.currency = record.fields[1];

    // Type (Deposit or Withdraw)
    txn.type = record.fields[2];

    // Amount
    try
    {
        std::string clean_amount = CleanNumericField(record.fields[3]);
        if (clean_amount.empty())
        {
            return std::nullopt;
        }
        txn.amount = std::stod(clean_amount);
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }

    // User ID
    try
    {
        txn.user_id = std::stoi(record.fields[4]);
    }
    catch (const std::invalid_argument&)
    {
        txn.user_id = 0;
    }
    catch (const std::out_of_range&)
    {
        txn.user_id = 0;
    }

    return txn;
}

} // namespace persistence