#include "actions/wallet/wallet_deposit_action.hpp"
#include "core/actions/action_helper.hpp"
#include <format>
WalletDepositAction::WalletDepositAction(std::shared_ptr<services::WalletService> wallet_service,
                                         std::shared_ptr<services::TradingService> trading_service)
    : wallet_service_(std::move(wallet_service)), trading_service_(std::move(trading_service))
{
}
wallet_forms::WalletOperationForm WalletDepositAction::CreateForm(ActionContext &context)
{
  auto allowed_currencies = trading_service_->GetAvailableProducts();
  return wallet_forms::WalletOperationForm(context.form_input_provider, context.output, allowed_currencies);
}
utils::ServiceResult<double> WalletDepositAction::ExecuteService(const dto::WalletOperation &data, ActionContext &context)
{
  double amount = std::stod(data.amount);
  return wallet_service_->Deposit(data.currency, amount);
}
void WalletDepositAction::DisplayResults(const utils::ServiceResult<double> &result, const dto::WalletOperation &data, ActionContext &context)
{
  actions::ActionHelper::DisplayResult(
      result.success, GetOperationName(), result.message, context,
      [&]()
      {
        context.output->WriteLine(std::format("Currency: {}", data.currency));
        context.output->WriteLine(std::format("New Balance: {:.2f}", *result.data));
      });
}
