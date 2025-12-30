#include "forms/user/validators/username_validator.hpp"

#include "app_constants.hpp"

#include <cctype>
#include <format>

namespace user_forms
{

std::optional<std::string> UsernameValidator::GetHint() const
{
    return std::format("{} digits", app::kUsernameLength);
}

form::ValidationResult UsernameValidator::Validate(const std::string& value,
                                                   const form::FormContext&) const
{
    if (value.size() != app::kUsernameLength)
    {
        return form::ValidationResult::Invalid(
            std::format("Username must be exactly {} digits", app::kUsernameLength));
    }

    for (unsigned char ch : value)
    {
        if (!std::isdigit(ch))
        {
            return form::ValidationResult::Invalid("Username must contain only digits");
        }
    }

    return form::ValidationResult::Valid();
}

} // namespace user_forms
