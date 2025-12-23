#include "actions/wallet/wallet_deposit_action.hpp"

#include <format>

#include "forms/wallet/wallet_operation_form.hpp"

WalletDepositAction::WalletDepositAction(std::shared_ptr<services::WalletService> wallet_service,
                                         std::shared_ptr<services::TradingService> trading_service)
    : wallet_service_(std::move(wallet_service)), trading_service_(std::move(trading_service))
{
}

void WalletDepositAction::Execute(ActionContext &context)
{
  // Controller: Prepare data from service
  auto allowed_currencies = trading_service_->GetAvailableCurrencies();

  dto::WalletOperation data;
  // Pass prepared data to view (form)
  wallet_forms::WalletOperationForm form(context.form_input_provider, context.output,
                                         allowed_currencies);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Deposit cancelled by user.");
    return;
  }

  // Use service to deposit funds
  double amount = std::stod(data.amount);
  auto result = wallet_service_->Deposit(data.currency, amount);
  DisplayResults(result, data.currency, context);
}

void WalletDepositAction::DisplayResults(const services::OperationResult &result,
                                         const std::string &currency,
                                         ActionContext &context) const
{
  context.output->WriteLine("");
  if (result.success)
  {
    context.output->WriteLine("=== Deposit Successful ===");
    context.output->WriteLine(std::format("Currency: {}", currency));
    context.output->WriteLine(std::format("New Balance: {:.2f}", result.new_balance));
  }
  else
  {
    context.output->WriteLine("=== Deposit Failed ===");
    context.output->WriteLine(std::format("Error: {}", result.message));
  }
}
