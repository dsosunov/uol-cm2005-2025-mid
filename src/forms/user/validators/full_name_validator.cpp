#include "forms/user/validators/full_name_validator.hpp"

#include <cctype>

namespace user_forms
{
form::ValidationResult FullNameValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Full name cannot be empty");
    }
    if (value.length() < 2)
    {
        return form::ValidationResult::Invalid("Full name must be at least 2 characters");
    }
    if (value.length() > 100)
    {
        return form::ValidationResult::Invalid("Full name must be at most 100 characters");
    }
    bool has_letter = false;
    for (char c : value)
    {
        if (std::isalpha(c))
        {
            has_letter = true;
            break;
        }
    }
    if (!has_letter)
    {
        return form::ValidationResult::Invalid("Full name must contain at least one letter");
    }
    return form::ValidationResult::Valid();
}
std::optional<std::string> FullNameValidator::GetHint() const
{
    return "Full name must be 2-100 characters and contain at least one letter";
}
} // namespace user_forms