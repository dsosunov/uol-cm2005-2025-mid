#include "actions/wallet/wallet_withdraw_action.hpp"

#include <format>

WalletWithdrawAction::WalletWithdrawAction(std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

wallet_forms::WalletOperationForm WalletWithdrawAction::CreateForm(ActionContext& context)
{
    std::set<std::string, std::less<>> allowed_currencies = {"USD", "EUR", "GBP", "JPY",
                                                             "CAD", "AUD", "CHF", "CNY"};
    return wallet_forms::WalletOperationForm(context.form_input_provider, context.output,
                                             allowed_currencies);
}
utils::ServiceResult<double> WalletWithdrawAction::ExecuteService(const dto::WalletOperation& data,
                                                                  ActionContext& context)
{
    double amount = std::stod(data.amount);
    return wallet_service_->Withdraw(data.currency, amount);
}
void WalletWithdrawAction::DisplayResults(const utils::ServiceResult<double>& result,
                                          const dto::WalletOperation& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        DisplayField("Currency", data.currency, context);
        if (result.data.has_value())
        {
            WriteLine(std::format("New Balance: {:.2f}", *result.data), context);
        }
        DisplayResultFooter(context);
    }
    else
    {
        DisplayFailureHeader(result.message, context);
        DisplayResultFooter(context);
    }
}
