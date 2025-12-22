#include "actions/wallet/wallet_withdraw_action.hpp"

#include <set>

#include "forms/wallet/wallet_operation_form.hpp"

static const std::set<std::string> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                         "JPY", "AUD", "CHF", "CNY"};

void WalletWithdrawAction::Execute(ActionContext &context)
{
  context.output->WriteLine("=== Withdraw Funds ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::WalletOperation data;
  wallet_forms::WalletOperationForm form(context.form_input_provider, context.output,
                                         kAllowedCurrencies);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Withdrawal cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void WalletWithdrawAction::DisplayResults(const dto::WalletOperation &data,
                                          ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Withdrawal Successful ===");
  context.output->WriteLine("Currency: " + data.currency);
  context.output->WriteLine("Amount: " + data.amount);
}
