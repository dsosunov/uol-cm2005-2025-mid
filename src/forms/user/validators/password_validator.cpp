#include "forms/user/validators/password_validator.hpp"

#include "app_constants.hpp"

#include <format>

namespace user_forms
{

form::ValidationResult PasswordValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Password cannot be empty");
    }

    if (value.length() < app::kPasswordMinLength)
    {
        return form::ValidationResult::Invalid(
            std::format("Password must be at least {} characters", app::kPasswordMinLength));
    }

    if (value.length() > app::kPasswordMaxLength)
    {
        return form::ValidationResult::Invalid(
            std::format("Password must be at most {} characters", app::kPasswordMaxLength));
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> PasswordValidator::GetHint() const
{
    return std::format("Password must be {}-{} characters", app::kPasswordMinLength,
                       app::kPasswordMaxLength);
}

} // namespace user_forms