#include "core/utils/time_utils.hpp"

#include <gtest/gtest.h>
#include <string>


TEST(TimestampParser, ParsesDateOnly)
{
    auto tp = utils::TimestampParser::Parse("2020-03-05");
    ASSERT_TRUE(tp.has_value());
    EXPECT_EQ(utils::FormatDate(*tp), "2020-03-05");
}

TEST(TimestampParser, ParsesTimestampWithMicrosecondsAndPadsTo6)
{
    auto tp = utils::TimestampParser::Parse("2020/03/05 12:34:56.123");
    ASSERT_TRUE(tp.has_value());
    EXPECT_EQ(utils::FormatTimestamp(*tp), "2020/03/05 12:34:56.123000");
}

TEST(TimestampParser, InvalidDateReturnsNullopt)
{
    EXPECT_FALSE(utils::TimestampParser::Parse("2020-13-01").has_value());
}

TEST(TimestampParser, MissingDelimitersReturnsNullopt)
{
    EXPECT_FALSE(utils::TimestampParser::Parse("20200305").has_value());
}
