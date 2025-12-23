#include "actions/wallet/wallet_deposit_action.hpp"

#include <format>
#include <set>

#include "forms/wallet/wallet_operation_form.hpp"

static const std::set<std::string, std::less<>> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                                      "JPY", "AUD", "CHF", "CNY"};

void WalletDepositAction::Execute(ActionContext &context)
{
  dto::WalletOperation data;
  wallet_forms::WalletOperationForm form(context.form_input_provider, context.output,
                                         kAllowedCurrencies);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Deposit cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void WalletDepositAction::DisplayResults(const dto::WalletOperation &data, ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Deposit Successful ===");
  context.output->WriteLine(std::format("Currency: {}", data.currency));
  context.output->WriteLine(std::format("Amount: {}", data.amount));
}
