#include "actions/wallet/wallet_deposit_action.hpp"

#include <set>

#include "forms/wallet/wallet_operation_form.hpp"

static const std::set<std::string> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                         "JPY", "AUD", "CHF", "CNY"};

void WalletDepositAction::Execute(ActionContext &context)
{
  context.output->WriteLine("=== Deposit Funds ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::WalletOperation data;
  wallet_forms::WalletOperationForm form(context.form_input_provider, context.output,
                                         kAllowedCurrencies);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Deposit cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void WalletDepositAction::DisplayResults(const dto::WalletOperation &data, ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Deposit Successful ===");
  context.output->WriteLine("Currency: " + data.currency);
  context.output->WriteLine("Amount: " + data.amount);
}
