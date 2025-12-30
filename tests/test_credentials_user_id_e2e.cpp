#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"
#include "persistence/credentials_data_adapter.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "persistence/user_data_adapter.hpp"
#include "services/authentication_context.hpp"
#include "services/authentication_service.hpp"
#include "services/credentials_service.hpp"
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

TEST(CredentialsUserIdE2E, RegistrationAssignsUniqueUserIdsAndShadowEntries)
{
    auto users_csv = WriteTempCsv("users", {});
    auto shadow_csv = WriteTempCsv("shadow", {});

    auto users_reader = std::make_shared<data::CsvReader>(users_csv);
    auto users_writer = std::make_shared<data::CsvWriter>(users_csv, false);
    auto user_adapter = std::make_shared<persistence::UserDataAdapter>(users_reader, users_writer);

    auto shadow_reader = std::make_shared<data::CsvReader>(shadow_csv);
    auto shadow_writer = std::make_shared<data::CsvWriter>(shadow_csv, false);
    auto credentials_adapter =
        std::make_shared<persistence::CredentialsDataAdapter>(shadow_reader, shadow_writer);
    auto credentials_service = std::make_shared<services::CredentialsService>(credentials_adapter);

    auto txns_csv = WriteTempCsv("transactions", {});
    auto txns_reader = std::make_shared<data::CsvReader>(txns_csv);
    auto txns_writer = std::make_shared<data::CsvWriter>(txns_csv, true);
    auto txn_adapter =
        std::make_shared<persistence::TransactionDataAdapter>(txns_reader, txns_writer);

    auto auth_ctx = std::make_shared<services::AuthenticationContext>();
    auto auth_svc = std::make_shared<services::AuthenticationService>(auth_ctx);
    auto user_service =
        std::make_shared<services::UserService>(user_adapter, auth_svc, credentials_service);

    auto transactions_service =
        std::make_shared<services::TransactionsService>(txn_adapter, user_service);
    auto wallet_service =
        std::make_shared<services::WalletService>(transactions_service, user_service);

    services::UserRegistrationService registration(user_adapter, wallet_service,
                                                   credentials_service);

    auto u1 = registration.RegisterUser("0000000001", "User One", "u1@example.com", "pass123");
    ASSERT_TRUE(u1.success) << u1.message;
    auto u2 = registration.RegisterUser("0000000002", "User Two", "u2@example.com", "pass456");
    ASSERT_TRUE(u2.success) << u2.message;

    ASSERT_TRUE(u1.data.has_value());
    ASSERT_TRUE(u2.data.has_value());

    EXPECT_NE(u1.data->id, 0);
    EXPECT_NE(u2.data->id, 0);
    EXPECT_NE(u1.data->id, u2.data->id);

    // Shadow must contain both ids.
    EXPECT_TRUE(credentials_service->ValidatePassword(u1.data->id, "pass123").success);
    EXPECT_TRUE(credentials_service->ValidatePassword(u2.data->id, "pass456").success);
}
