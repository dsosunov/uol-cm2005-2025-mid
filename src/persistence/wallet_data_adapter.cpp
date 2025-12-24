#include "persistence/wallet_data_adapter.hpp"
namespace persistence
{
    WalletDataAdapter::WalletDataAdapter(std::shared_ptr<data::CsvReader> reader) : reader_(reader)
    {
    }
    bool WalletDataAdapter::IsValid() const
    {
        return reader_ && reader_->FileExists();
    }
    std::map<std::string, double, std::less<>> WalletDataAdapter::ReadBalances(int user_id) const
    {
        std::map<std::string, double, std::less<>> balances;
        if (!IsValid())
        {
            return balances;
        }
        reader_->ReadWithProcessor([&balances, user_id](const data::CsvRecord &record)
                                   {
            if (record.fields.size() < 5)
            {
                return;
            }
            try
            {
                int record_user_id = std::stoi(record.fields[2]);
                if (record_user_id != user_id)
                {
                    return;
                }
                std::string currency = record.fields[1];
                double balance = std::stod(record.fields[4]);
                balances[currency] = balance;
            }
            catch (...)
            {
            } });
        return balances;
    }
    bool WalletDataAdapter::WriteBalances(data::CsvWriter &writer, int user_id,
                                          const std::map<std::string, double, std::less<>> &balances)
    {
        for (const auto &[currency, balance] : balances)
        {
            data::CsvRecord record;
            record.fields.reserve(5);
            record.fields.push_back("");
            record.fields.push_back(currency);
            record.fields.push_back(std::to_string(user_id));
            record.fields.push_back("0");
            record.fields.push_back(std::to_string(balance));
            if (!writer.Write(record))
            {
                return false;
            }
        }
        return writer.Flush();
    }
}