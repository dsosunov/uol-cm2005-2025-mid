#include "actions/wallet/wallet_withdraw_action.hpp"

#include <format>
#include <set>

#include "forms/wallet/wallet_operation_form.hpp"

static const std::set<std::string, std::less<>> kAllowedCurrencies = {"USD", "CAD", "EUR", "GBP",
                                                                      "JPY", "AUD", "CHF", "CNY"};

void WalletWithdrawAction::Execute(ActionContext &context)
{
  dto::WalletOperation data;
  wallet_forms::WalletOperationForm form(context.form_input_provider, context.output,
                                         kAllowedCurrencies);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Withdrawal cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void WalletWithdrawAction::DisplayResults(const dto::WalletOperation &data,
                                          ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Withdrawal Successful ===");
  context.output->WriteLine(std::format("Currency: {}", data.currency));
  context.output->WriteLine(std::format("Amount: {}", data.amount));
}
