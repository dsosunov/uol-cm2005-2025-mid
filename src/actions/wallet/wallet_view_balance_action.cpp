#include "actions/wallet/wallet_view_balance_action.hpp"

#include <format>

WalletViewBalanceAction::WalletViewBalanceAction(
    std::shared_ptr<services::WalletService> wallet_service)
    : wallet_service_(std::move(wallet_service))
{
}

shared_forms::EmptyForm WalletViewBalanceAction::CreateForm(ActionContext& context)
{
    return shared_forms::EmptyForm();
}

utils::ServiceResult<std::map<std::string, double, std::less<>>> WalletViewBalanceAction::
    ExecuteService(const EmptyRequest& data, ActionContext& context)
{
    return wallet_service_->GetBalances();
}

void WalletViewBalanceAction::DisplayResults(
    const utils::ServiceResult<std::map<std::string, double, std::less<>>>& result,
    const EmptyRequest& data, ActionContext& context)
{
    if (!result.success || !result.data.has_value())
    {
        DisplayFailureHeader(result.message, context);
        return;
    }

    const auto& balances = *result.data;
    DisplaySuccessHeader(context);

    if (balances.empty())
    {
        WriteLine("No balances available.", context);
    }
    else
    {
        for (const auto& [currency, amount] : balances)
        {
            std::string symbol = "$";
            if (currency == "EUR")
                symbol = "€";
            else if (currency == "GBP")
                symbol = "£";
            else if (currency == "JPY")
                symbol = "¥";
            WriteLine(std::format("{}: {}{:.2f}", currency, symbol, amount), context);
        }
        WriteEmptyLine(context);
        auto total_result = wallet_service_->GetTotalBalanceInUSD();
        if (total_result.success && total_result.data.has_value())
        {
            WriteLine(std::format("Total (USD equivalent): ${:.2f}", *total_result.data), context);
        }
    }
    DisplayResultFooter(context);
}
