#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "persistence/user_data_adapter.hpp"
#include "services/authentication_context.hpp"
#include "services/authentication_service.hpp"
#include "services/trading_activities_service.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"
#include "services/user_registration_service.hpp"
#include "services/user_service.hpp"
#include "services/wallet_service.hpp"

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace
{
std::filesystem::path WriteTempCsv(std::string_view prefix, const std::vector<std::string>& lines)
{
    std::random_device rd;
    auto suffix =
        std::format("{}_{}",
                    static_cast<unsigned long long>(
                        std::chrono::high_resolution_clock::now().time_since_epoch().count()),
                    static_cast<unsigned long long>(rd()));

    auto path = std::filesystem::temp_directory_path() / std::format("{}_{}.csv", prefix, suffix);

    std::ofstream out(path, std::ios::trunc);
    for (const auto& line : lines)
    {
        out << line << "\n";
    }
    out.flush();
    out.close();

    return path;
}
} // namespace

TEST(TradingSimulationInitialBalancesE2E, NewUserCanSimulateThreeTimes)
{
    auto users_csv = WriteTempCsv("users", {});
    auto txns_csv = WriteTempCsv("transactions", {});
    auto orders_csv = WriteTempCsv("orders", {});

    auto users_reader = std::make_shared<data::CsvReader>(users_csv);
    auto users_writer = std::make_shared<data::CsvWriter>(users_csv, false);
    auto user_adapter = std::make_shared<persistence::UserDataAdapter>(users_reader, users_writer);

    auto auth_ctx = std::make_shared<services::AuthenticationContext>();
    auto auth_svc = std::make_shared<services::AuthenticationService>(auth_ctx);

    auto user_service = std::make_shared<services::UserService>(user_adapter, auth_svc);

    auto txns_reader = std::make_shared<data::CsvReader>(txns_csv);
    auto txns_writer = std::make_shared<data::CsvWriter>(txns_csv, true);
    auto txn_adapter =
        std::make_shared<persistence::TransactionDataAdapter>(txns_reader, txns_writer);
    auto transactions_service =
        std::make_shared<services::TransactionsService>(txn_adapter, user_service);

    auto wallet_service =
        std::make_shared<services::WalletService>(transactions_service, user_service);

    services::UserRegistrationService registration(user_adapter, wallet_service);

    auto reg = registration.RegisterUser("0000000001", "Test User", "t@example.com", "pass");
    ASSERT_TRUE(reg.success) << reg.message;
    ASSERT_TRUE(reg.data.has_value());

    const auto& user = *reg.data;
    auth_svc->SetCurrentUser(
        services::AuthenticatedUser{user.id, user.username, user.full_name, user.email});

    auto orders_reader = std::make_shared<data::CsvReader>(orders_csv);
    auto orders_writer = std::make_shared<data::CsvWriter>(orders_csv, true);
    auto trading_adapter =
        std::make_shared<persistence::TradingDataAdapter>(orders_reader, orders_writer);
    auto trading_service = std::make_shared<services::TradingService>(trading_adapter);

    services::TradingActivitiesService sim(auth_svc, wallet_service, trading_service);

    for (int i = 0; i < 3; ++i)
    {
        auto result = sim.SimulateUserTradingActivities(5);
        ASSERT_TRUE(result.success) << result.message;
    }
}
