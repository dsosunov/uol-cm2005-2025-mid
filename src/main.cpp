#include "actions/generation/auto_generate_trades_action.hpp"
#include "actions/summary/candlestick_summary_action.hpp"
#include "actions/transaction/transaction_activity_summary_action.hpp"
#include "actions/transaction/transaction_show_by_pair_action.hpp"
#include "actions/transaction/transaction_show_last5_action.hpp"
#include "actions/user/user_login_action.hpp"
#include "actions/user/user_register_action.hpp"
#include "actions/user/user_reset_action.hpp"
#include "actions/wallet/wallet_deposit_action.hpp"
#include "actions/wallet/wallet_view_balance_action.hpp"
#include "actions/wallet/wallet_withdraw_action.hpp"
#include "core/ui/menu/menu_builder.hpp"
#include "core/ui/menu/menu_engine.hpp"
#include "service_container.hpp"

#include <memory>

std::unique_ptr<MenuNode> BuildMenu(const ServiceContainer& container)
{
    auto user_service = container.Resolve<services::UserService>();
    auto wallet_service = container.Resolve<services::WalletService>();
    auto transactions_service = container.Resolve<services::TransactionsService>();
    auto trading_service = container.Resolve<services::TradingService>();
    return MenuBuilder("Trading Platform")
        .AddLeaf("Candlestick summary", std::make_unique<CandlestickSummaryAction>(trading_service))
        .AddBranch("User")
        .AddLeaf("Register", std::make_unique<UserRegisterAction>(user_service))
        .AddLeaf("Login", std::make_unique<UserLoginAction>(user_service))
        .AddLeaf("Reset / Remind", std::make_unique<UserResetAction>(user_service))
        .Parent()
        .AddBranch("Wallet")
        .AddLeaf("View balance", std::make_unique<WalletViewBalanceAction>(wallet_service))
        .AddLeaf("Deposit", std::make_unique<WalletDepositAction>(wallet_service, trading_service))
        .AddLeaf("Take away",
                 std::make_unique<WalletWithdrawAction>(wallet_service, trading_service))
        .Parent()
        .AddBranch("Transactions")
        .AddLeaf("Show last 5", std::make_unique<TransactionShowLast5Action>(transactions_service))
        .AddLeaf("Show by product pair", std::make_unique<TransactionShowByPairAction>(
                                             transactions_service, trading_service))
        .AddLeaf("Show activity summary", std::make_unique<TransactionActivitySummaryAction>(
                                              transactions_service, trading_service))
        .Parent()
        .AddLeaf("Auto-generate trades",
                 std::make_unique<AutoGenerateTradesAction>(trading_service))
        .Build();
}
int main()
{
    ServiceContainer container;
    auto menu = BuildMenu(container);
    MenuEngine engine(std::move(menu), container.Resolve<MenuRenderer>(),
                      container.Resolve<MenuInput>(), container.Resolve<ActionContext>());
    engine.Run();
    return 0;
}