#include "actions/wallet/wallet_withdraw_action.hpp"

#include <format>

WalletWithdrawAction::WalletWithdrawAction(std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

wallet_forms::WalletOperationForm WalletWithdrawAction::CreateForm(ActionContext& context)
{
    std::set<std::string, std::less<>> allowed_currencies;

    // Allow withdrawing only currencies present in the user's wallet history.
    // (Effectively: currencies the user has deposited/held at some point.)
    auto balances_result = wallet_service_->GetBalances();
    if (balances_result.success && balances_result.data.has_value())
    {
        for (const auto& [currency, amount] : *balances_result.data)
        {
            (void)amount;
            allowed_currencies.insert(currency);
        }
    }

    if (allowed_currencies.empty())
    {
        WriteLine("No deposited currencies available to withdraw. Deposit first.", context);
    }

    return wallet_forms::WalletOperationForm(context.form_input_provider, context.output,
                                             allowed_currencies);
}
utils::ServiceResult<double> WalletWithdrawAction::ExecuteService(const dto::WalletOperation& data,
                                                                  ActionContext& context)
{
    if (!context.auth_service->IsAuthenticated())
    {
        return utils::ServiceResult<double>::Failure("Please log in first to access the wallet");
    }

    double amount = std::stod(data.amount);
    return wallet_service_->Withdraw(data.currency, amount);
}
void WalletWithdrawAction::DisplayResults(const utils::ServiceResult<double>& result,
                                          const dto::WalletOperation& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        double amount = std::stod(data.amount);

        WriteLine(std::format("{:<12} {:<12} {:<12}", "Currency", "Amount", "New Balance"),
                  context);
        WriteLine(std::string(12 + 1 + 12 + 1 + 12, '-'), context);

        WriteLine(std::format("{:<12} {:<12.2f} {:<12.2f}", data.currency, amount,
                              result.data.value_or(0.0)),
                  context);
        DisplayResultFooter(context);
    }
    else
    {
        DisplayFailureHeader(result.message, context);
        DisplayResultFooter(context);
    }
}
