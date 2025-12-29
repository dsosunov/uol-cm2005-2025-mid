#include "services/wallet_service.hpp"

#include "core/utils/time_utils.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "services/authentication_service.hpp"
#include "services/transactions_service.hpp"

#include <stdexcept>

namespace services
{

WalletService::WalletService(std::shared_ptr<persistence::TransactionDataAdapter> adapter,
                             std::shared_ptr<services::AuthenticationService> auth_service)
    : adapter_(std::move(adapter)), auth_service_(std::move(auth_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("TransactionDataAdapter is required");
    }
    if (!auth_service_)
    {
        throw std::invalid_argument("AuthenticationService is required");
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

utils::ServiceResult<std::map<std::string, double, std::less<>>> WalletService::GetBalances() const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<std::map<std::string, double, std::less<>>>::Failure(
            user_result.message);
    }

    int effective_id = user_result.data.value().id;
    auto balances = CalculateBalances(effective_id);
    return {true, "Balances retrieved successfully", balances};
}

utils::ServiceResult<double> WalletService::GetBalance(std::string_view currency) const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<double>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;
    auto balances = CalculateBalances(effective_id);

    if (auto it = balances.find(currency); it != balances.end())
    {
        return {true, "Balance retrieved successfully", it->second};
    }

    return {true, "No balance found for currency", 0.0};
}

utils::ServiceResult<double> WalletService::GetTotalBalanceInUSD() const
{
    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<double>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;
    auto balances = CalculateBalances(effective_id);

    if (auto it = balances.find("USD"); it != balances.end())
    {
        return {true, "USD balance retrieved successfully", it->second};
    }

    return {true, "No USD balance found", 0.0};
}

utils::ServiceResult<double> WalletService::Deposit(std::string_view currency, double amount)
{
    if (amount <= 0)
    {
        return utils::ServiceResult<double>::Failure("Amount must be positive");
    }

    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<double>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;

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

utils::ServiceResult<double> WalletService::Withdraw(std::string_view currency, double amount)
{
    if (amount <= 0)
    {
        return utils::ServiceResult<double>::Failure("Amount must be positive");
    }

    auto user_result = auth_service_->GetAuthenticatedUser();
    if (!user_result.success)
    {
        return utils::ServiceResult<double>::Failure(user_result.message);
    }

    int effective_id = user_result.data.value().id;

    auto balance_result = GetBalance(currency);
    if (!balance_result.success || balance_result.data.value() < amount)
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