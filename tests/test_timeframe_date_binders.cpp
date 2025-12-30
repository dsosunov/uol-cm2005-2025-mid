#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/form_context.hpp"
#include "dto/candlestick_query.hpp"
#include "dto/constants.hpp"

#include <any>
#include <chrono>
#include <gtest/gtest.h>
#include <optional>
#include <string>

class TimeframeDateBindersTest : public ::testing::Test
{
  protected:
    static utils::TimePoint MakeTp(int year, unsigned month, unsigned day)
    {
        auto ymd = std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day};
        auto days = std::chrono::sys_days{ymd};
        return std::chrono::time_point_cast<std::chrono::microseconds>(days);
    }
};

TEST_F(TimeframeDateBindersTest, MonthlyStartParsesYearMonthToFirstDay)
{
    dto::CandlestickQuery query{};
    std::any target = std::ref(query);

    form::FormContext ctx;
    ctx.SetValue("timeframe", dto::Timeframe::Monthly);

    form::StartDateFieldBinder<dto::CandlestickQuery> binder(&dto::CandlestickQuery::start_date);
    std::any value = std::string{"2020-03"};

    binder(target, value, ctx);

    ASSERT_TRUE(query.start_date.has_value());
    EXPECT_EQ(*query.start_date, MakeTp(2020, 3, 1));
}

TEST_F(TimeframeDateBindersTest, MonthlyEndParsesYearMonthToNextMonthExclusive)
{
    dto::CandlestickQuery query{};
    std::any target = std::ref(query);

    form::FormContext ctx;
    ctx.SetValue("timeframe", dto::Timeframe::Monthly);

    form::EndDateFieldBinder<dto::CandlestickQuery> binder(&dto::CandlestickQuery::end_date);
    std::any value = std::string{"2020-03"};

    binder(target, value, ctx);

    ASSERT_TRUE(query.end_date.has_value());
    EXPECT_EQ(*query.end_date, MakeTp(2020, 4, 1));
}

TEST_F(TimeframeDateBindersTest, YearlyStartParsesYearToJanFirst)
{
    dto::CandlestickQuery query{};
    std::any target = std::ref(query);

    form::FormContext ctx;
    ctx.SetValue("timeframe", dto::Timeframe::Yearly);

    form::StartDateFieldBinder<dto::CandlestickQuery> binder(&dto::CandlestickQuery::start_date);
    std::any value = std::string{"2020"};

    binder(target, value, ctx);

    ASSERT_TRUE(query.start_date.has_value());
    EXPECT_EQ(*query.start_date, MakeTp(2020, 1, 1));
}

TEST_F(TimeframeDateBindersTest, YearlyEndParsesYearToNextJanFirstExclusive)
{
    dto::CandlestickQuery query{};
    std::any target = std::ref(query);

    form::FormContext ctx;
    ctx.SetValue("timeframe", dto::Timeframe::Yearly);

    form::EndDateFieldBinder<dto::CandlestickQuery> binder(&dto::CandlestickQuery::end_date);
    std::any value = std::string{"2020"};

    binder(target, value, ctx);

    ASSERT_TRUE(query.end_date.has_value());
    EXPECT_EQ(*query.end_date, MakeTp(2021, 1, 1));
}

TEST_F(TimeframeDateBindersTest, DailyEndIsNextDayExclusive)
{
    dto::CandlestickQuery query{};
    std::any target = std::ref(query);

    form::FormContext ctx;
    ctx.SetValue("timeframe", dto::Timeframe::Daily);

    form::EndDateFieldBinder<dto::CandlestickQuery> binder(&dto::CandlestickQuery::end_date);
    std::any value = std::string{"2020-03-05"};

    binder(target, value, ctx);

    ASSERT_TRUE(query.end_date.has_value());
    EXPECT_EQ(*query.end_date, MakeTp(2020, 3, 6));
}
