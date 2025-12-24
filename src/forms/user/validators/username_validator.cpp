#include "forms/user/validators/username_validator.hpp"

#include <cctype>

namespace user_forms
{
form::ValidationResult UsernameValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Username cannot be empty");
    }

    if (value.length() < 3)
    {
        return form::ValidationResult::Invalid("Username must be at least 3 characters");
    }

    if (value.length() > 20)
    {
        return form::ValidationResult::Invalid("Username must be at most 20 characters");
    }

    for (char c : value)
    {
        if (!std::isalnum(c) && c != '_')
        {
            return form::ValidationResult::Invalid(
                "Username can only contain letters, numbers, and underscores");
        }
    }
    return form::ValidationResult::Valid();
}
std::optional<std::string> UsernameValidator::GetHint() const
{
    return "3-20 characters, letters, numbers, and underscores only";
}
} // namespace user_forms