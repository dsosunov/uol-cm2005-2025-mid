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
            if (int record_user_id = std::stoi(record.order_type); record_user_id != user_id)
            {
                return;
            }
            std::string currency = record.product;
            double balance = std::stod(record.amount);
            balances[currency] = balance; });
        return balances;
    }
    bool WalletDataAdapter::WriteBalances(data::CsvWriter &writer, int user_id,
                                          const std::map<std::string, double, std::less<>> &balances)
    {
        for (const auto &[currency, balance] : balances)
        {
            data::CsvRecord record;
            record.timestamp = "";
            record.product = currency;
            record.order_type = std::to_string(user_id);
            record.price = "0";
            record.amount = std::to_string(balance);
            if (!writer.Write(record))
            {
                return false;
            }
        }
        return writer.Flush();
    }
}