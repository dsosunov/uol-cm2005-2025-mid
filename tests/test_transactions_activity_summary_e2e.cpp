#include "core/data/csv_reader.hpp"
#include "persistence/credentials_data_adapter.hpp"
#include "persistence/transaction_data_adapter.hpp"
#include "persistence/user_data_adapter.hpp"
#include "services/authentication_context.hpp"
#include "services/authentication_service.hpp"
#include "services/credentials_service.hpp"
#include "services/transactions_service.hpp"
#include "services/user_service.hpp"

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace
{
utils::TimePoint MakeTp(int year, unsigned month, unsigned day, int hour = 0, int minute = 0,
                        int second = 0)
{
    auto ymd = std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day};
    auto days = std::chrono::sys_days{ymd};
    auto hms =
        std::chrono::hours{hour} + std::chrono::minutes{minute} + std::chrono::seconds{second};
    return std::chrono::time_point_cast<std::chrono::microseconds>(days + hms);
}

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

std::shared_ptr<services::UserService> MakeUserService(const std::filesystem::path& users_csv,
                                                       const std::filesystem::path& shadow_csv)
{
    auto reader = std::make_shared<data::CsvReader>(users_csv);
    auto adapter = std::make_shared<persistence::UserDataAdapter>(reader, nullptr);

    auto shadow_reader = std::make_shared<data::CsvReader>(shadow_csv);
    auto credentials_adapter =
        std::make_shared<persistence::CredentialsDataAdapter>(shadow_reader, nullptr);
    auto credentials_service = std::make_shared<services::CredentialsService>(credentials_adapter);

    auto auth_ctx = std::make_shared<services::AuthenticationContext>();
    auto auth_svc = std::make_shared<services::AuthenticationService>(auth_ctx);

    return std::make_shared<services::UserService>(adapter, auth_svc, credentials_service);
}

services::TransactionsService MakeTransactionsService(const std::filesystem::path& txns_csv,
                                                      std::shared_ptr<services::UserService> us)
{
    auto reader = std::make_shared<data::CsvReader>(txns_csv);
    auto adapter = std::make_shared<persistence::TransactionDataAdapter>(reader, nullptr);
    return services::TransactionsService(adapter, std::move(us));
}
} // namespace

TEST(TransactionsActivitySummaryE2E, DailyGroupsByDay)
{
    auto users_csv = WriteTempCsv("users", {
                                               "1,testuser,Test User,test@example.com",
                                           });
    auto shadow_csv = WriteTempCsv("shadow", {});

    auto txns_csv = WriteTempCsv("txns", {
                                             "2020/03/05 10:00:00,USD,Deposit,10.0,1",
                                             "2020/03/05 11:00:00,USD,Withdraw,5.0,1",
                                             "2020/03/06 09:00:00,USD,Deposit,2.0,1",
                                         });

    auto user_service = MakeUserService(users_csv, shadow_csv);
    auto service = MakeTransactionsService(txns_csv, user_service);

    auto result = service.GetActivitySummary(1, dto::Timeframe::Daily,
                                             std::optional<utils::TimePoint>{MakeTp(2020, 3, 5)},
                                             std::optional<utils::TimePoint>{MakeTp(2020, 3, 7)});

    ASSERT_TRUE(result.success);
    ASSERT_TRUE(result.data.has_value());

    const auto& stats = *result.data;
    EXPECT_EQ(stats.total_transactions, 3);
    EXPECT_DOUBLE_EQ(stats.total_volume, 17.0);
    EXPECT_DOUBLE_EQ(stats.average_transaction_size, 17.0 / 3.0);

    ASSERT_EQ(stats.per_period.size(), 2u);

    EXPECT_EQ(stats.per_period[0].period, "2020-03-05");
    EXPECT_EQ(stats.per_period[0].total_transactions, 2);
    EXPECT_DOUBLE_EQ(stats.per_period[0].total_volume, 15.0);
    EXPECT_DOUBLE_EQ(stats.per_period[0].average_transaction_size, 7.5);

    EXPECT_EQ(stats.per_period[1].period, "2020-03-06");
    EXPECT_EQ(stats.per_period[1].total_transactions, 1);
    EXPECT_DOUBLE_EQ(stats.per_period[1].total_volume, 2.0);
    EXPECT_DOUBLE_EQ(stats.per_period[1].average_transaction_size, 2.0);
}

TEST(TransactionsActivitySummaryE2E, MonthlyGroupsByMonth)
{
    auto users_csv = WriteTempCsv("users", {
                                               "1,testuser,Test User,test@example.com",
                                           });
    auto shadow_csv = WriteTempCsv("shadow", {});

    auto txns_csv = WriteTempCsv("txns", {
                                             "2020/03/31 23:59:59,USD,Deposit,1.0,1",
                                             "2020/04/01 00:00:00,USD,Deposit,2.0,1",
                                         });

    auto user_service = MakeUserService(users_csv, shadow_csv);
    auto service = MakeTransactionsService(txns_csv, user_service);

    auto result = service.GetActivitySummary(1, dto::Timeframe::Monthly,
                                             std::optional<utils::TimePoint>{MakeTp(2020, 3, 1)},
                                             std::optional<utils::TimePoint>{MakeTp(2020, 5, 1)});

    ASSERT_TRUE(result.success);
    ASSERT_TRUE(result.data.has_value());

    const auto& stats = *result.data;
    EXPECT_EQ(stats.total_transactions, 2);
    EXPECT_DOUBLE_EQ(stats.total_volume, 3.0);

    ASSERT_EQ(stats.per_period.size(), 2u);
    EXPECT_EQ(stats.per_period[0].period, "2020-03");
    EXPECT_EQ(stats.per_period[0].total_transactions, 1);
    EXPECT_DOUBLE_EQ(stats.per_period[0].total_volume, 1.0);

    EXPECT_EQ(stats.per_period[1].period, "2020-04");
    EXPECT_EQ(stats.per_period[1].total_transactions, 1);
    EXPECT_DOUBLE_EQ(stats.per_period[1].total_volume, 2.0);
}

TEST(TransactionsActivitySummaryE2E, YearlyGroupsByYear)
{
    auto users_csv = WriteTempCsv("users", {
                                               "1,testuser,Test User,test@example.com",
                                           });
    auto shadow_csv = WriteTempCsv("shadow", {});

    auto txns_csv = WriteTempCsv("txns", {
                                             "2019/12/31 23:00:00,USD,Deposit,1.0,1",
                                             "2020/01/01 00:00:00,USD,Deposit,2.0,1",
                                             "2020/06/01 00:00:00,USD,Deposit,3.0,1",
                                         });

    auto user_service = MakeUserService(users_csv, shadow_csv);
    auto service = MakeTransactionsService(txns_csv, user_service);

    auto result = service.GetActivitySummary(1, dto::Timeframe::Yearly,
                                             std::optional<utils::TimePoint>{MakeTp(2019, 1, 1)},
                                             std::optional<utils::TimePoint>{MakeTp(2021, 1, 1)});

    ASSERT_TRUE(result.success);
    ASSERT_TRUE(result.data.has_value());

    const auto& stats = *result.data;
    EXPECT_EQ(stats.total_transactions, 3);
    EXPECT_DOUBLE_EQ(stats.total_volume, 6.0);

    ASSERT_EQ(stats.per_period.size(), 2u);
    EXPECT_EQ(stats.per_period[0].period, "2019");
    EXPECT_EQ(stats.per_period[0].total_transactions, 1);
    EXPECT_DOUBLE_EQ(stats.per_period[0].total_volume, 1.0);

    EXPECT_EQ(stats.per_period[1].period, "2020");
    EXPECT_EQ(stats.per_period[1].total_transactions, 2);
    EXPECT_DOUBLE_EQ(stats.per_period[1].total_volume, 5.0);
    EXPECT_DOUBLE_EQ(stats.per_period[1].average_transaction_size, 2.5);
}
