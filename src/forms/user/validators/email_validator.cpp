#include "forms/user/validators/email_validator.hpp"

namespace user_forms
{

form::ValidationResult EmailValidator::Validate(const std::string& value,
                                                const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Email cannot be empty");
    }

    size_t at_pos = value.find('@');
    if (at_pos == std::string::npos || at_pos == 0 || at_pos == value.length() - 1)
    {
        return form::ValidationResult::Invalid("Invalid email format");
    }

    if (size_t dot_pos = value.find('.', at_pos);
        dot_pos == std::string::npos || dot_pos == value.length() - 1)
    {
        return form::ValidationResult::Invalid("Invalid email format");
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> EmailValidator::GetHint() const
{
    return "Valid email format (e.g., user@example.com)";
}

} // namespace user_forms