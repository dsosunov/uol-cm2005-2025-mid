#include <gtest/gtest.h>
#include "forms/user/validators/full_name_validator.hpp"
#include "core/ui/form/form_context.hpp"

using namespace user_forms;
using namespace form;
class FullNameValidatorTest : public ::testing::Test
{
protected:
    FullNameValidator validator;
    FormContext context;
};
TEST_F(FullNameValidatorTest, RejectsEmptyString)
{
    auto result = validator.Validate("", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name cannot be empty");
}
TEST_F(FullNameValidatorTest, RejectsSingleCharacter)
{
    auto result = validator.Validate("A", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name must be at least 2 characters");
}

TEST_F(FullNameValidatorTest, AcceptsTwoCharacters)
{
    auto result = validator.Validate("Al", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}
TEST_F(FullNameValidatorTest, AcceptsExactly100Characters)
{
    std::string name_100_chars(100, 'A');
    auto result = validator.Validate(name_100_chars, context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, Rejects101Characters)
{
    std::string name_101_chars(101, 'A');
    auto result = validator.Validate(name_101_chars, context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name must be at most 100 characters");
}
TEST_F(FullNameValidatorTest, RejectsOnlyNumbers)
{
    auto result = validator.Validate("12345", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name must contain at least one letter");
}

TEST_F(FullNameValidatorTest, RejectsOnlySpecialCharacters)
{
    auto result = validator.Validate("!@#$%", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name must contain at least one letter");
}

TEST_F(FullNameValidatorTest, RejectsSpacesOnly)
{
    auto result = validator.Validate("     ", context);
    EXPECT_FALSE(result.is_valid);
    EXPECT_EQ(result.error_message, "Full name must contain at least one letter");
}
TEST_F(FullNameValidatorTest, AcceptsSimpleName)
{
    auto result = validator.Validate("John", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsFullNameWithSpace)
{
    auto result = validator.Validate("John Doe", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithMultipleSpaces)
{
    auto result = validator.Validate("John Michael Doe", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithHyphen)
{
    auto result = validator.Validate("Mary-Jane", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithApostrophe)
{
    auto result = validator.Validate("O'Brien", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithNumbers)
{
    auto result = validator.Validate("John Doe 3rd", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithPeriods)
{
    auto result = validator.Validate("Dr. Smith", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsNameWithAccents)
{
    auto result = validator.Validate("José García", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsLowercaseName)
{
    auto result = validator.Validate("john doe", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsUppercaseName)
{
    auto result = validator.Validate("JOHN DOE", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(FullNameValidatorTest, AcceptsMixedCaseName)
{
    auto result = validator.Validate("JoHn DoE", context);
    EXPECT_TRUE(result.is_valid);
    EXPECT_TRUE(result.error_message.empty());
}
