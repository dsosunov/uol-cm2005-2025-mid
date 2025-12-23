#include "actions/wallet/wallet_view_balance_action.hpp"

#include <format>

WalletViewBalanceAction::WalletViewBalanceAction(
    std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

void WalletViewBalanceAction::Execute(ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Wallet Balances ===");
  auto balances = wallet_service_->GetBalances();

  if (balances.empty())
  {
    context.output->WriteLine("No balances available.");
  }
  else
  {
    for (const auto &[currency, amount] : balances)
    {
      std::string symbol = "$";
      if (currency == "EUR")
        symbol = "€";
      else if (currency == "GBP")
        symbol = "£";
      else if (currency == "JPY")
        symbol = "¥";

      context.output->WriteLine(std::format("{}: {}{:.2f}", currency, symbol, amount));
    }

    context.output->WriteLine("");
    double total_usd = wallet_service_->GetTotalBalanceInUSD();
    context.output->WriteLine(std::format("Total (USD equivalent): ${:.2f}", total_usd));
  }
}
