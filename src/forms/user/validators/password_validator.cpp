#include "forms/user/validators/password_validator.hpp"
namespace user_forms
{
    form::ValidationResult PasswordValidator::Validate(const std::string &value,
                                                       const form::FormContext &context) const
    {
        if (value.empty())
        {
            return form::ValidationResult::Invalid("Password cannot be empty");
        }
        if (value.length() < 6)
        {
            return form::ValidationResult::Invalid("Password must be at least 6 characters");
        }
        if (value.length() > 50)
        {
            return form::ValidationResult::Invalid("Password must be at most 50 characters");
        }
        return form::ValidationResult::Valid();
    }
    std::optional<std::string> PasswordValidator::GetHint() const
    {
        return "Password must be 6-50 characters";
    }
}