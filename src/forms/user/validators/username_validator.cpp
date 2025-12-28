#include "forms/user/validators/username_validator.hpp"

#include <cctype>

namespace user_forms
{

std::optional<std::string> UsernameValidator::GetHint() const
{
    return "10 digits";
}

form::ValidationResult UsernameValidator::Validate(const std::string& value,
                                                   const form::FormContext&) const
{
    if (value.size() != 10)
    {
        return form::ValidationResult::Invalid("Username must be exactly 10 digits");
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
