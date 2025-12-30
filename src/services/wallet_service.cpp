#include "services/wallet_service.hpp"

#include "app_constants.hpp"
#include "core/utils/time_utils.hpp"
#include "services/transactions_service.hpp"
#include "services/user_service.hpp"

#include <stdexcept>

namespace services
{
WalletService::WalletService(std::shared_ptr<services::TransactionsService> transactions_service,
                             std::shared_ptr<services::UserService> user_service)
    : transactions_service_(std::move(transactions_service)), user_service_(std::move(user_service))
{
    if (!transactions_service_)
    {
        throw std::invalid_argument("TransactionsService is required");
    }
    if (!user_service_)
    {
        throw std::invalid_argument("UserService is required");
    }
}

std::map<std::string, double, std::less<>> WalletService::CalculateBalances(int user_id) const
{
    std::map<std::string, double, std::less<>> balances;

    auto all = transactions_service_->GetAllTransactions(user_id);
    if (!all.success || !all.data.has_value())
    {
        return balances;
    }

    for (const auto& txn : *all.data)
    {
        if (txn.type == kWalletTransactionTypeDeposit)
        {
            balances[txn.currency] += txn.amount;
        }
        else if (txn.type == kWalletTransactionTypeWithdraw)
        {
            balances[txn.currency] -= txn.amount;
        }
    }

    return balances;
}

double WalletService::ExchangeRateToUSD(std::string_view currency) const
{
    if (auto it = app::kExchangeRateToUSD.find(currency); it != app::kExchangeRateToUSD.end())
    {
        return it->second;
    }

    return app::kDefaultExchangeRateToUSD;
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

    double total_usd = 0.0;
    for (const auto& [currency, amount] : balances)
    {
        if (amount == 0.0)
        {
            continue;
        }

        const double rate_to_usd = ExchangeRateToUSD(currency);

        total_usd += amount * rate_to_usd;
    }

    return utils::ServiceResult<double>::Success(total_usd,
                                                 "Total wallet balance in USD calculated");
}

utils::ServiceResult<double> WalletService::Deposit(int user_id, std::string_view currency,
                                                    double amount) const
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

    auto add = transactions_service_->AddTransaction(
        user_id, currency, kWalletTransactionTypeDeposit, amount, utils::Now());
    if (!add.success)
    {
        return utils::ServiceResult<double>::Failure(add.message);
    }

    auto balances = CalculateBalances(user_id);
    double new_balance = balances[std::string(currency)];

    return utils::ServiceResult<double>::Success(new_balance, "Deposit successful");
}

utils::ServiceResult<double> WalletService::Withdraw(int user_id, std::string_view currency,
                                                     double amount) const
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

    auto add = transactions_service_->AddTransaction(
        user_id, currency, kWalletTransactionTypeWithdraw, amount, utils::Now());
    if (!add.success)
    {
        return utils::ServiceResult<double>::Failure(add.message);
    }

    auto balances = CalculateBalances(user_id);
    double new_balance = balances[std::string(currency)];

    return utils::ServiceResult<double>::Success(new_balance, "Withdrawal successful");
}

} // namespace services