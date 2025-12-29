#include "actions/wallet/wallet_deposit_action.hpp"

#include <format>

WalletDepositAction::WalletDepositAction(std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

wallet_forms::WalletOperationForm WalletDepositAction::CreateForm(ActionContext& context)
{
    // Deposit supports only trading currencies.
    std::set<std::string, std::less<>> allowed_currencies = {"BTC", "DOGE", "ETH", "USDT"};
    return wallet_forms::WalletOperationForm(context.form_input_provider, context.output,
                                             allowed_currencies);
}
utils::ServiceResult<double> WalletDepositAction::ExecuteService(const dto::WalletOperation& data,
                                                                 ActionContext& context)
{
    auto au = context.auth_service->GetAuthenticatedUser();
    if (!au.success)
    {
        return utils::ServiceResult<double>::Failure("Please log in first to access the wallet");
    }

    double amount = std::stod(data.amount);
    return wallet_service_->Deposit(au.data->id, data.currency, amount);
}
void WalletDepositAction::DisplayResults(const utils::ServiceResult<double>& result,
                                         const dto::WalletOperation& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        double amount = std::stod(data.amount);

        WriteLine(std::format("{:<12} {:<12} {:<12}", "Currency", "Amount", "New Balance"),
                  context);
        WriteLine(std::string(12 + 1 + 12 + 1 + 12, '-'), context);

        WriteLine(std::format("{:<12} {:<12.4f} {:<12.4f}", data.currency, amount,
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
