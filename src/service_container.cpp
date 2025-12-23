#include "service_container.hpp"

#include "core/actions/action_context.hpp"
#include "core/data/csv_reader.hpp"
#include "core/ui/form/menu_form_input_provider.hpp"
#include "core/ui/io/standard_input.hpp"
#include "core/ui/io/standard_output.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"
#include "services/user_service.hpp"
#include "services/wallet_service.hpp"

ServiceContainer::ServiceContainer()
{
    auto csv_reader = std::make_shared<data::CsvReader>("data/20200317.csv");
    auto trading_adapter = std::make_shared<persistence::TradingDataAdapter>(csv_reader);

    Register(std::make_shared<services::UserService>());
    Register(std::make_shared<services::WalletService>());
    Register(std::make_shared<services::TransactionsService>());
    Register(std::make_shared<services::TradingService>(trading_adapter));
    auto output = std::make_shared<StandardOutput>();
    auto input = std::make_shared<StandardInput>();
    auto renderer = std::make_shared<MenuRenderer>(output);
    auto menu_input = std::make_shared<MenuInput>(input, renderer);
    auto form_input_provider =
        std::make_shared<form::MenuFormInputProvider>(input, renderer, menu_input);
    auto context = std::make_shared<ActionContext>(output, form_input_provider);

    Register(output);
    Register(input);
    Register(renderer);
    Register(menu_input);
    Register(form_input_provider);
    Register(context);
}
