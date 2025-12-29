#include "services/wallet_service.hpp"

#include "core/utils/time_utils.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "services/transactions_service.hpp" // for WalletTransaction
#include "services/user_service.hpp"

#include <stdexcept>

namespace services
{

WalletService::WalletService(std::shared_ptr<persistence::TransactionDataAdapter> adapter,
                             std::shared_ptr<services::UserService> user_service)
    : adapter_(std::move(adapter)), user_service_(std::move(user_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("TransactionDataAdapter is required");
    }
    if (!user_service_)
    {
        throw std::invalid_argument("UserService is required");
    }
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
    int user_id) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<std::map<std::string, double, std::less<>>>::Failure(
            validate.message);
    }

    auto balances = CalculateBalances(user_id);
    return {true, "Balances retrieved successfully", balances};
}

utils::ServiceResult<double> WalletService::GetBalance(int user_id, std::string_view currency) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<double>::Failure(validate.message);
    }

    auto balances = CalculateBalances(user_id);

    if (auto it = balances.find(currency); it != balances.end())
    {
        return {true, "Balance retrieved successfully", it->second};
    }

    return {true, "No balance found for currency", 0.0};
}

utils::ServiceResult<double> WalletService::GetTotalBalanceInUSD(int user_id) const
{
    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<double>::Failure(validate.message);
    }

    auto balances = CalculateBalances(user_id);

    if (auto it = balances.find("USD"); it != balances.end())
    {
        return {true, "USD balance retrieved successfully", it->second};
    }

    return {true, "No USD balance found", 0.0};
}

utils::ServiceResult<double> WalletService::Deposit(int user_id, std::string_view currency,
                                                    double amount)
{
    if (amount <= 0)
    {
        return utils::ServiceResult<double>::Failure("Amount must be positive");
    }

    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<double>::Failure(validate.message);
    }

    WalletTransaction txn;
    txn.currency = std::string(currency);
    txn.type = "Deposit";
    txn.amount = amount;
    txn.timestamp = utils::Now();
    txn.user_id = user_id;

    if (!adapter_->Add(txn))
    {
        return utils::ServiceResult<double>::Failure("Failed to write transaction");
    }

    auto balances = CalculateBalances(user_id);
    double new_balance = balances[std::string(currency)];

    return utils::ServiceResult<double>::Success(new_balance, "Deposit successful");
}

utils::ServiceResult<double> WalletService::Withdraw(int user_id, std::string_view currency,
                                                     double amount)
{
    if (amount <= 0)
    {
        return utils::ServiceResult<double>::Failure("Amount must be positive");
    }

    auto validate = user_service_->ValidateUserId(user_id);
    if (!validate.success)
    {
        return utils::ServiceResult<double>::Failure(validate.message);
    }

    if (auto balance_result = GetBalance(user_id, currency);
        !balance_result.success || balance_result.data.value() < amount)
    {
        return utils::ServiceResult<double>::Failure("Insufficient balance");
    }

    WalletTransaction txn;
    txn.currency = std::string(currency);
    txn.type = "Withdraw";
    txn.amount = amount;
    txn.timestamp = utils::Now();
    txn.user_id = user_id;

    if (!adapter_->Add(txn))
    {
        return utils::ServiceResult<double>::Failure("Failed to write transaction");
    }

    auto balances = CalculateBalances(user_id);
    double new_balance = balances[std::string(currency)];

    return utils::ServiceResult<double>::Success(new_balance, "Withdrawal successful");
}

} // namespace services