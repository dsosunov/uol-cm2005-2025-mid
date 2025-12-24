#include "actions/wallet/wallet_deposit_action.hpp"

#include <format>

WalletDepositAction::WalletDepositAction(std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

wallet_forms::WalletOperationForm WalletDepositAction::CreateForm(ActionContext& context)
{
    std::set<std::string, std::less<>> allowed_currencies = {"USD", "EUR", "GBP", "JPY",
                                                             "CAD", "AUD", "CHF", "CNY"};
    return wallet_forms::WalletOperationForm(context.form_input_provider, context.output,
                                             allowed_currencies);
}
utils::ServiceResult<double> WalletDepositAction::ExecuteService(const dto::WalletOperation& data,
                                                                 ActionContext& context)
{
    double amount = std::stod(data.amount);
    return wallet_service_->Deposit(data.currency, amount);
}
void WalletDepositAction::DisplayResults(const utils::ServiceResult<double>& result,
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
