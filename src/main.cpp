#include "actions/summary/candlestick_summary_action.hpp"
#include "actions/trading/trading_simulation_action.hpp"
#include "actions/transaction/transaction_activity_summary_action.hpp"
#include "actions/transaction/transaction_show_by_currency_action.hpp"
#include "actions/transaction/transaction_show_last5_action.hpp"
#include "actions/user/user_login_action.hpp"
#include "actions/user/user_logoff_action.hpp"
#include "actions/user/user_register_action.hpp"
#include "actions/user/user_remind_username_action.hpp"
#include "actions/user/user_reset_action.hpp"
#include "actions/wallet/wallet_deposit_action.hpp"
#include "actions/wallet/wallet_view_balance_action.hpp"
#include "actions/wallet/wallet_withdraw_action.hpp"
#include "core/ui/menu/menu_builder.hpp"
#include "core/ui/menu/menu_engine.hpp"
#include "service_container.hpp"
#include "services/trading_activities_service.hpp"
#include "services/user_registration_service.hpp"

#include <memory>

// Menu structure (auth-only nodes marked with [auth]):
//
// Trading Platform
//  - Register
//  - Login
//     - Log in
//     - Remind a user name
//     - Change a password by name
//  - Candlestick summary
//  - Wallet [auth]
//     - View balance
//     - Deposit
//     - Withdraw
//     - Transactions [auth]
//        - Show last 5
//        - Show by currency
//        - Show summary
//  - Trading [auth]
//  - Log off [auth]
std::unique_ptr<MenuNode> BuildMenu(const ServiceContainer& container)
{
    auto user_service = container.Resolve<services::UserService>();
    auto user_registration_service = container.Resolve<services::UserRegistrationService>();
    auto wallet_service = container.Resolve<services::WalletService>();
    auto transactions_service = container.Resolve<services::TransactionsService>();
    auto trading_service = container.Resolve<services::TradingService>();
    auto trading_activities_service = container.Resolve<services::TradingActivitiesService>();

    return MenuBuilder("Trading Platform")
        .AddLeaf("Register", std::make_unique<UserRegisterAction>(user_registration_service))
        .AddBranch("Login")
        .AddLeaf("Log in", std::make_unique<UserLoginAction>(user_service))
        .AddLeaf("Remind a user name", std::make_unique<UserRemindUsernameAction>(user_service))
        .AddLeaf("Change a password by name", std::make_unique<UserResetAction>(user_service))
        .Parent()
        .AddLeaf("Candlestick summary", std::make_unique<CandlestickSummaryAction>(trading_service))
        .AddBranch("Wallet", true)
        .AddLeaf("View balance", std::make_unique<WalletViewBalanceAction>(wallet_service), true)
        .AddLeaf("Deposit", std::make_unique<WalletDepositAction>(wallet_service), true)
        .AddLeaf("Withdraw", std::make_unique<WalletWithdrawAction>(wallet_service), true)
        .AddBranch("Transactions", true)
        .AddLeaf("Show last 5", std::make_unique<TransactionShowLast5Action>(transactions_service),
                 true)
        .AddLeaf("Show by currency",
                 std::make_unique<TransactionShowByCurrencyAction>(transactions_service), true)
        .AddLeaf("Show summary",
                 std::make_unique<TransactionActivitySummaryAction>(transactions_service), true)
        .Parent()
        .Parent()
        .AddLeaf("Trading", std::make_unique<TradingSimulationAction>(trading_activities_service),
                 true)
        .AddLeaf("Log off", std::make_unique<UserLogoffAction>(user_service), true)
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