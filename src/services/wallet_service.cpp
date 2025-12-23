#include "services/wallet_service.hpp"
#include "persistence/wallet_data_adapter.hpp"
#include "core/data/csv_writer.hpp"
#include "core/utils/service_utils.hpp"
namespace services
{
    WalletService::WalletService()
    {
        balances_[1]["USD"] = 1234.56;
        balances_[1]["EUR"] = 987.65;
        balances_[1]["GBP"] = 543.21;
        balances_[1]["JPY"] = 50000.00;
        balances_[1]["CAD"] = 1500.00;
    }
    WalletService::WalletService(std::shared_ptr<persistence::WalletDataAdapter> reader,
                                 std::shared_ptr<data::CsvWriter> writer)
        : reader_(reader), writer_(writer)
    {
        if (reader_)
        {
            balances_[default_user_id_] = reader_->ReadBalances(default_user_id_);
        }
    }
    int WalletService::GetEffectiveUserId(std::optional<int> user_id) const
    {
        return ServiceUtils::GetEffectiveUserId(user_id, default_user_id_);
    }
    std::map<std::string, double, std::less<>> WalletService::GetBalances(std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        if (reader_ && balances_.find(effective_id) == balances_.end())
        {
            balances_[effective_id] = reader_->ReadBalances(effective_id);
        }
        if (auto it = balances_.find(effective_id); it != balances_.end())
        {
            return it->second;
        }
        return {};
    }
    double WalletService::GetBalance(std::string_view currency, std::optional<int> user_id) const
    {
        int effective_id = GetEffectiveUserId(user_id);
        if (auto user_it = balances_.find(effective_id); user_it != balances_.end())
        {
            auto currency_it = user_it->second.find(currency);
            if (currency_it != user_it->second.end())
            {
                return currency_it->second;
            }
        }
        return 0.0;
    }
    double WalletService::GetTotalBalanceInUSD(std::optional<int> user_id) const
    {
        std::map<std::string, double, std::less<>> rates = {
            {"USD", 1.0}, {"EUR", 1.09}, {"GBP", 1.27}, {"JPY", 0.0067}, {"CAD", 0.73}, {"AUD", 0.65}, {"CHF", 1.13}, {"CNY", 0.14}};
        int effective_id = GetEffectiveUserId(user_id);
        auto user_it = balances_.find(effective_id);
        if (user_it == balances_.end())
        {
            return 0.0;
        }
        double total_usd = 0.0;
        for (const auto &[currency, amount] : user_it->second)
        {
            auto rate_it = rates.find(currency);
            if (rate_it != rates.end())
            {
                total_usd += amount * rate_it->second;
            }
        }
        return total_usd;
    }
    utils::ServiceResult<double> WalletService::Deposit(std::string_view currency, double amount,
                                                        std::optional<int> user_id)
    {
        if (amount <= 0)
        {
            return utils::ServiceResult<double>::Failure("Amount must be positive");
        }
        int effective_id = GetEffectiveUserId(user_id);
        std::string currency_str(currency);
        balances_[effective_id][currency_str] += amount;
        double new_balance = balances_[effective_id][currency_str];
        SaveBalances(effective_id);
        return utils::ServiceResult<double>::Success(new_balance, "Deposit successful");
    }
    utils::ServiceResult<double> WalletService::Withdraw(std::string_view currency, double amount,
                                                         std::optional<int> user_id)
    {
        if (amount <= 0)
        {
            return utils::ServiceResult<double>::Failure("Amount must be positive");
        }
        int effective_id = GetEffectiveUserId(user_id);
        if (double current_balance = GetBalance(currency, user_id); current_balance < amount)
        {
            return utils::ServiceResult<double>::Failure("Insufficient balance");
        }
        std::string currency_str(currency);
        balances_[effective_id][currency_str] -= amount;
        double new_balance = balances_[effective_id][currency_str];
        SaveBalances(effective_id);
        return utils::ServiceResult<double>::Success(new_balance, "Withdrawal successful");
    }
    void WalletService::SaveBalances(int user_id)
    {
        if (writer_ && balances_.find(user_id) != balances_.end())
        {
            persistence::WalletDataAdapter::WriteBalances(*writer_, user_id, balances_[user_id]);
        }
    }
}