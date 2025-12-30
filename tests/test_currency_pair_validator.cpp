#include "core/ui/form/form_context.hpp"
#include "forms/shared/validators/currency_pair_validator.hpp"

#include <gtest/gtest.h>

using namespace forms::shared;
using namespace form;

class CurrencyPairValidatorTest : public ::testing::Test
{
  protected:
    FormContext context;
};

TEST_F(CurrencyPairValidatorTest, AcceptsValidPair)
{
    CurrencyPairValidator validator({"BTC", "ETH", "USDT"});

    auto result = validator.Validate("btc/usdt", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(CurrencyPairValidatorTest, RejectsMissingSlash)
{
    CurrencyPairValidator validator({"BTC", "ETH", "USDT"});

    auto result = validator.Validate("BTCUSDT", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Format must be CUR1/CUR2. Allowed currencies: BTC, ETH, USDT");
}

TEST_F(CurrencyPairValidatorTest, RejectsEmptySide)
{
    CurrencyPairValidator validator({"BTC", "ETH", "USDT"});

    auto result = validator.Validate("/USDT", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Format must be CUR1/CUR2. Allowed currencies: BTC, ETH, USDT");
}

TEST_F(CurrencyPairValidatorTest, RejectsSameCurrencyPair)
{
    CurrencyPairValidator validator({"BTC", "ETH", "USDT"});

    auto result = validator.Validate("USDT/USDT", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(
        result.error_message,
        "Currency pair must contain two different currencies. Allowed currencies: BTC, ETH, USDT");
}

TEST_F(CurrencyPairValidatorTest, RejectsSameCurrencyPairWithXXXSample)
{
    CurrencyPairValidator validator({"BTC", "XXX"});

    auto result = validator.Validate("XXX/XXX", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message,
              "Currency pair must contain two different currencies. Allowed currencies: BTC, XXX");
}
