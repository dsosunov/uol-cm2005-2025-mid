#include <memory>

#include "core/ui/io/standard_input.hpp"
#include "core/ui/io/standard_output.hpp"
#include "core/ui/form/menu_form_input_provider.hpp"
#include "core/actions/action_context.hpp"
#include "actions/candlestick_summary_action.hpp"
#include "actions/user_register_action.hpp"
#include "actions/user_login_action.hpp"
#include "actions/user_reset_action.hpp"
#include "actions/wallet_view_balance_action.hpp"
#include "actions/wallet_deposit_action.hpp"
#include "actions/wallet_withdraw_action.hpp"
#include "actions/transaction_show_last5_action.hpp"
#include "actions/transaction_show_by_pair_action.hpp"
#include "actions/transaction_activity_summary_action.hpp"
#include "actions/auto_generate_trades_action.hpp"
#include "core/ui/menu/menu_builder.hpp"
#include "core/ui/menu/menu_engine.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"

std::unique_ptr<MenuNode> BuildMenu()
{
    return MenuBuilder("Trading Platform")
        .AddLeaf("Candlestick summary", std::make_unique<CandlestickSummaryAction>())
        .AddBranch("User")
            .AddLeaf("Register", std::make_unique<UserRegisterAction>())
            .AddLeaf("Login", std::make_unique<UserLoginAction>())
            .AddLeaf("Reset / Remind", std::make_unique<UserResetAction>())
            .Parent()
        .AddBranch("Wallet")
            .AddLeaf("View balance", std::make_unique<WalletViewBalanceAction>())
            .AddLeaf("Deposit", std::make_unique<WalletDepositAction>())
            .AddLeaf("Take away", std::make_unique<WalletWithdrawAction>())
            .Parent()
        .AddBranch("Transactions")
            .AddLeaf("Show last 5", std::make_unique<TransactionShowLast5Action>())
            .AddLeaf("Show by product pair", std::make_unique<TransactionShowByPairAction>())
            .AddLeaf("Show activity summary", std::make_unique<TransactionActivitySummaryAction>())
            .Parent()
        .AddLeaf("Auto-generate trades", std::make_unique<AutoGenerateTradesAction>())
        .Build();
}

int main()
{
    auto output = std::make_shared<StandardOutput>();
    auto input = std::make_shared<StandardInput>();
    auto renderer = std::make_shared<MenuRenderer>(output);
    auto menuInput = std::make_shared<MenuInput>(input, renderer);

    // Create FormInputProvider
    auto formInputProvider = std::make_shared<form::MenuFormInputProvider>(
        input, output, renderer, menuInput);

    // Create ActionContext with dependencies
    auto context = std::make_shared<ActionContext>(output, formInputProvider);

    auto menu = BuildMenu();
    MenuEngine engine(std::move(menu), renderer, menuInput, context);
    engine.Run();
    return 0;
}
