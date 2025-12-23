#include "actions/wallet/wallet_view_balance_action.hpp"

void WalletViewBalanceAction::Execute(ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("USD: $1,234.56");
  context.output->WriteLine("EUR: €987.65");
  context.output->WriteLine("GBP: £543.21");
  context.output->WriteLine("");
  context.output->WriteLine("Total (USD equivalent): $2,850.42");
}
