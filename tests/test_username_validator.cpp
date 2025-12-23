#include <gtest/gtest.h>
#include "forms/user/validators/username_validator.hpp"
#include "core/ui/form/form_context.hpp"

using namespace user_forms;
using namespace form;
class UsernameValidatorTest : public ::testing::Test
{
protected:
    UsernameValidator validator;
    FormContext context;
};
TEST_F(UsernameValidatorTest, RejectsEmptyString)
{
    auto result = validator.Validate("", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username cannot be empty");
}
TEST_F(UsernameValidatorTest, RejectsTwoCharacters)
{
    auto result = validator.Validate("ab", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username must be at least 3 characters");
}

TEST_F(UsernameValidatorTest, RejectsSingleCharacter)
{
    auto result = validator.Validate("a", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username must be at least 3 characters");
}

TEST_F(UsernameValidatorTest, AcceptsThreeCharacters)
{
    auto result = validator.Validate("abc", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}
TEST_F(UsernameValidatorTest, AcceptsExactly20Characters)
{
    std::string username_20_chars(20, 'a');
    auto result = validator.Validate(username_20_chars, context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, Rejects21Characters)
{
    std::string username_21_chars(21, 'a');
    auto result = validator.Validate(username_21_chars, context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username must be at most 20 characters");
}
TEST_F(UsernameValidatorTest, RejectsSpaces)
{
    auto result = validator.Validate("user name", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}

TEST_F(UsernameValidatorTest, RejectsHyphens)
{
    auto result = validator.Validate("user-name", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}

TEST_F(UsernameValidatorTest, RejectsPeriods)
{
    auto result = validator.Validate("user.name", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}

TEST_F(UsernameValidatorTest, RejectsAtSymbol)
{
    auto result = validator.Validate("user@name", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}

TEST_F(UsernameValidatorTest, RejectsSpecialCharacters)
{
    auto result = validator.Validate("user!name", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}

TEST_F(UsernameValidatorTest, RejectsMultipleSpecialCharacters)
{
    auto result = validator.Validate("u$er#name%", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Username can only contain letters, numbers, and underscores");
}
TEST_F(UsernameValidatorTest, AcceptsLettersOnly)
{
    auto result = validator.Validate("username", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsNumbersOnly)
{
    auto result = validator.Validate("123456", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsUnderscoresOnly)
{
    auto result = validator.Validate("___", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsLettersAndNumbers)
{
    auto result = validator.Validate("user123", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsLettersAndUnderscores)
{
    auto result = validator.Validate("user_name", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsNumbersAndUnderscores)
{
    auto result = validator.Validate("123_456", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsLettersNumbersAndUnderscores)
{
    auto result = validator.Validate("user_123", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsUnderscoreAtStart)
{
    auto result = validator.Validate("_username", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsUnderscoreAtEnd)
{
    auto result = validator.Validate("username_", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsMultipleUnderscores)
{
    auto result = validator.Validate("user__name", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsUppercaseLetters)
{
    auto result = validator.Validate("USERNAME", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsMixedCase)
{
    auto result = validator.Validate("UserName123", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(UsernameValidatorTest, AcceptsComplexValidUsername)
{
    auto result = validator.Validate("User_Name_123", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}
