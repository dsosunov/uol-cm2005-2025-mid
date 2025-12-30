#include "forms/user/validators/full_name_validator.hpp"

#include "app_constants.hpp"

#include <cctype>
#include <format>

namespace user_forms
{

form::ValidationResult FullNameValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Full name cannot be empty");
    }

    if (value.length() < app::kFullNameMinLength)
    {
        return form::ValidationResult::Invalid(
            std::format("Full name must be at least {} characters", app::kFullNameMinLength));
    }

    if (value.length() > app::kFullNameMaxLength)
    {
        return form::ValidationResult::Invalid(
            std::format("Full name must be at most {} characters", app::kFullNameMaxLength));
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
    return std::format("Full name must be {}-{} characters and contain at least one letter",
                       app::kFullNameMinLength, app::kFullNameMaxLength);
}

} // namespace user_forms