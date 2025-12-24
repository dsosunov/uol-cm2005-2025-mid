#include "services/wallet_service.hpp"

#include "core/utils/service_utils.hpp"
#include "core/utils/time_utils.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "services/transactions_service.hpp"

namespace services
{

WalletService::WalletService(std::shared_ptr<persistence::TransactionDataAdapter> adapter)
    : adapter_(adapter)
{
}

int WalletService::GetEffectiveUserId(std::optional<int> user_id) const
{
    return ServiceUtils::GetEffectiveUserId(user_id, default_user_id_);
}

std::map<std::string, double, std::less<>> WalletService::CalculateBalances(int user_id) const
{
    std::map<std::string, double, std::less<>> balances;

    adapter_->ReadWithProcessor([&balances, user_id](const WalletTransaction& txn) {
        if (txn.user_id == user_id)
        {
            if (txn.type == "Deposit")
            {
                balances[txn.currency] += txn.amount;
            }
            else if (txn.type == "Withdraw")
            {
                balances[txn.currency] -= txn.amount;
            }
        }
    });

    return balances;
}

utils::ServiceResult<std::map<std::string, double, std::less<>>> WalletService::GetBalances(
    std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    auto balances = CalculateBalances(effective_id);
    return {true, "Balances retrieved successfully", balances};
}

utils::ServiceResult<double> WalletService::GetBalance(std::string_view currency,
                                                       std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    auto balances = CalculateBalances(effective_id);

    if (auto it = balances.find(currency); it != balances.end())
    {
        return {true, "Balance retrieved successfully", it->second};
    }

    return {true, "No balance found for currency", 0.0};
}

utils::ServiceResult<double> WalletService::GetTotalBalanceInUSD(std::optional<int> user_id) const
{
    int effective_id = GetEffectiveUserId(user_id);
    auto balances = CalculateBalances(effective_id);

    if (auto it = balances.find("USD"); it != balances.end())
    {
        return {true, "USD balance retrieved successfully", it->second};
    }

    return {true, "No USD balance found", 0.0};
}

utils::ServiceResult<double> WalletService::Deposit(std::string_view currency, double amount,
                                                    std::optional<int> user_id)
{
    if (amount <= 0)
    {
        return utils::ServiceResult<double>::Failure("Amount must be positive");
    }

    int effective_id = GetEffectiveUserId(user_id);

    WalletTransaction txn;
    txn.currency = std::string(currency);
    txn.type = "Deposit";
    txn.amount = amount;
    txn.timestamp = utils::Now();
    txn.user_id = effective_id;

    if (!adapter_->Add(txn))
    {
        return utils::ServiceResult<double>::Failure("Failed to write transaction");
    }

    auto balances = CalculateBalances(effective_id);
    double new_balance = balances[std::string(currency)];

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

    auto balance_result = GetBalance(currency, user_id);
    if (!balance_result.success || !balance_result.data.has_value() ||
        *balance_result.data < amount)
    {
        return utils::ServiceResult<double>::Failure("Insufficient balance");
    }

    WalletTransaction txn;
    txn.currency = std::string(currency);
    txn.type = "Withdraw";
    txn.amount = amount;
    txn.timestamp = utils::Now();
    txn.user_id = effective_id;

    if (!adapter_->Add(txn))
    {
        return utils::ServiceResult<double>::Failure("Failed to write transaction");
    }

    auto balances = CalculateBalances(effective_id);
    double new_balance = balances[std::string(currency)];

    return utils::ServiceResult<double>::Success(new_balance, "Withdrawal successful");
}

} // namespace services