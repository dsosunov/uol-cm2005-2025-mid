#include "core/data/csv_reader.hpp"
#include "persistence/trading_data_adapter.hpp"
#include "services/analytic_service.hpp"
#include "services/trading_service.hpp"

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

static utils::TimePoint MakeTp(int year, unsigned month, unsigned day, int hour = 0, int minute = 0,
                               int second = 0)
{
    auto ymd = std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day};
    auto days = std::chrono::sys_days{ymd};
    auto hms =
        std::chrono::hours{hour} + std::chrono::minutes{minute} + std::chrono::seconds{second};
    return std::chrono::time_point_cast<std::chrono::microseconds>(days + hms);
}

static std::filesystem::path WriteTempCsv(const std::vector<std::string>& lines)
{
    std::random_device rd;
    auto suffix =
        std::format("{}_{}",
                    static_cast<unsigned long long>(
                        std::chrono::high_resolution_clock::now().time_since_epoch().count()),
                    static_cast<unsigned long long>(rd()));

    auto path = std::filesystem::temp_directory_path() /
                std::format("candlestick_service_e2e_{}.csv", suffix);

    std::ofstream out(path, std::ios::trunc);
    for (const auto& line : lines)
    {
        out << line << "\n";
    }
    out.flush();
    out.close();

    return path;
}

TEST(CandlestickServiceE2E, DailyOpenCloseUseTimestampsNotInputOrder)
{
    auto csv_path = WriteTempCsv({
        "2020/03/05 15:00:00,USD/EUR,ask,100.0,1.0",
        "2020/03/05 09:00:00,USD/EUR,ask,90.0,2.0",
        "2020/03/05 20:00:00,USD/EUR,ask,110.0,3.0",
    });

    auto reader = std::make_shared<data::CsvReader>(csv_path);
    auto adapter = std::make_shared<persistence::TradingDataAdapter>(reader);
    auto trading_service = std::make_shared<services::TradingService>(adapter);
    services::AnalyticService service(trading_service);

    auto result =
        service.GetCandlestickSummary("USD", "EUR", dto::OrderType::Asks, dto::Timeframe::Daily,
                                      std::optional<utils::TimePoint>{MakeTp(2020, 3, 5)},
                                      std::optional<utils::TimePoint>{MakeTp(2020, 3, 6)});

    ASSERT_TRUE(result.success);
    ASSERT_TRUE(result.data.has_value());
    ASSERT_EQ(result.data->candlesticks.size(), 1u);

    const auto& c = result.data->candlesticks.front();
    EXPECT_EQ(c.period, "2020-03-05");
    EXPECT_DOUBLE_EQ(c.open, 90.0);
    EXPECT_DOUBLE_EQ(c.close, 110.0);
    EXPECT_DOUBLE_EQ(c.high, 110.0);
    EXPECT_DOUBLE_EQ(c.low, 90.0);
    EXPECT_DOUBLE_EQ(c.total_volume, 6.0);
    EXPECT_DOUBLE_EQ(c.avg_volume, 2.0);
    EXPECT_EQ(c.trade_count, 3);
}

TEST(CandlestickServiceE2E, MonthlyEndExclusiveExcludesNextMonthAtMidnight)
{
    auto csv_path = WriteTempCsv({
        "2020/03/31 23:59:59,USD/EUR,ask,100.0,1.0",
        "2020/04/01 00:00:00,USD/EUR,ask,999.0,1.0",
    });

    auto reader = std::make_shared<data::CsvReader>(csv_path);
    auto adapter = std::make_shared<persistence::TradingDataAdapter>(reader);
    auto trading_service = std::make_shared<services::TradingService>(adapter);
    services::AnalyticService service(trading_service);

    auto result =
        service.GetCandlestickSummary("USD", "EUR", dto::OrderType::Asks, dto::Timeframe::Monthly,
                                      std::optional<utils::TimePoint>{MakeTp(2020, 3, 1)},
                                      std::optional<utils::TimePoint>{MakeTp(2020, 4, 1)});

    ASSERT_TRUE(result.success);
    ASSERT_TRUE(result.data.has_value());
    ASSERT_EQ(result.data->candlesticks.size(), 1u);

    const auto& c = result.data->candlesticks.front();
    EXPECT_EQ(c.period, "2020-03");
    EXPECT_DOUBLE_EQ(c.open, 100.0);
    EXPECT_DOUBLE_EQ(c.close, 100.0);
    EXPECT_DOUBLE_EQ(c.high, 100.0);
    EXPECT_DOUBLE_EQ(c.low, 100.0);
    EXPECT_DOUBLE_EQ(c.total_volume, 1.0);
    EXPECT_DOUBLE_EQ(c.avg_volume, 1.0);
    EXPECT_EQ(c.trade_count, 1);
}

TEST(CandlestickServiceE2E, FiltersByOrderType)
{
    auto csv_path = WriteTempCsv({
        "2020/03/05 09:00:00,USD/EUR,ask,90.0,1.0",
        "2020/03/05 10:00:00,USD/EUR,bid,80.0,1.0",
        "2020/03/05 11:00:00,USD/EUR,ask,100.0,1.0",
    });

    auto reader = std::make_shared<data::CsvReader>(csv_path);
    auto adapter = std::make_shared<persistence::TradingDataAdapter>(reader);
    auto trading_service = std::make_shared<services::TradingService>(adapter);
    services::AnalyticService service(trading_service);

    auto asks =
        service.GetCandlestickSummary("USD", "EUR", dto::OrderType::Asks, dto::Timeframe::Daily,
                                      std::optional<utils::TimePoint>{MakeTp(2020, 3, 5)},
                                      std::optional<utils::TimePoint>{MakeTp(2020, 3, 6)});

    ASSERT_TRUE(asks.success);
    ASSERT_TRUE(asks.data.has_value());
    ASSERT_EQ(asks.data->candlesticks.size(), 1u);

    const auto& c = asks.data->candlesticks.front();
    EXPECT_DOUBLE_EQ(c.open, 90.0);
    EXPECT_DOUBLE_EQ(c.close, 100.0);
    EXPECT_DOUBLE_EQ(c.high, 100.0);
    EXPECT_DOUBLE_EQ(c.low, 90.0);
    EXPECT_EQ(c.trade_count, 2);
}
