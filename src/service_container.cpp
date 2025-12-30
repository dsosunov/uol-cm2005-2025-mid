#include "service_container.hpp"

#include "core/actions/action_context.hpp"
#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include "core/ui/form/menu_form_input_provider.hpp"
#include "core/ui/io/standard_input.hpp"
#include "core/ui/io/standard_output.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "persistence/user_data_adapter.hpp"
#include "services/analytic_service.hpp"
#include "services/authentication_context.hpp"
#include "services/authentication_service.hpp"
#include "services/trading_activities_service.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"
#include "services/user_registration_service.hpp"
#include "services/user_service.hpp"
#include "services/wallet_service.hpp"

ServiceContainer::ServiceContainer()
{
    auto auth_context = std::make_shared<services::AuthenticationContext>();
    auto auth_service = std::make_shared<services::AuthenticationService>(auth_context);

    auto csv_reader = std::make_shared<data::CsvReader>("data/20200317.csv");
    auto csv_writer = std::make_shared<data::CsvWriter>("data/20200317.csv", true);
    auto trading_adapter =
        std::make_shared<persistence::TradingDataAdapter>(csv_reader, csv_writer);

    auto user_csv_reader = std::make_shared<data::CsvReader>("data/users.csv");
    auto user_csv_writer = std::make_shared<data::CsvWriter>("data/users.csv", false);
    auto user_adapter =
        std::make_shared<persistence::UserDataAdapter>(user_csv_reader, user_csv_writer);

    auto transaction_csv_reader = std::make_shared<data::CsvReader>("data/transactions.csv");
    auto transaction_csv_writer = std::make_shared<data::CsvWriter>("data/transactions.csv", true);
    auto transaction_adapter = std::make_shared<persistence::TransactionDataAdapter>(
        transaction_csv_reader, transaction_csv_writer);

    Register(auth_context);
    Register(auth_service);

    auto user_service = std::make_shared<services::UserService>(user_adapter, auth_service);
    Register(user_service);

    auto transactions_service =
        std::make_shared<services::TransactionsService>(transaction_adapter, user_service);
    Register(transactions_service);

    auto wallet_service =
        std::make_shared<services::WalletService>(transactions_service, user_service);
    Register(wallet_service);

    auto user_registration_service =
        std::make_shared<services::UserRegistrationService>(user_adapter, wallet_service);
    Register(user_registration_service);
    auto trading_service = std::make_shared<services::TradingService>(trading_adapter);
    Register(trading_service);

    auto analytic_service = std::make_shared<services::AnalyticService>(trading_service);
    Register(analytic_service);
    Register(std::make_shared<services::TradingActivitiesService>(
        auth_service, Resolve<services::WalletService>(), trading_service));

    auto output = std::make_shared<StandardOutput>();
    auto input = std::make_shared<StandardInput>();
    auto renderer = std::make_shared<MenuRenderer>(output, auth_service);
    auto menu_input = std::make_shared<MenuInput>(input, renderer, auth_service);

    auto form_input_provider =
        std::make_shared<form::MenuFormInputProvider>(input, renderer, menu_input);
    auto context = std::make_shared<ActionContext>(output, form_input_provider, auth_service);

    Register(output);
    Register(input);
    Register(renderer);
    Register(menu_input);
    Register(form_input_provider);
    Register(context);
}