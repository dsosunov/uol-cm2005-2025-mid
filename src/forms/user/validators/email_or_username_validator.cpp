#include "forms/user/validators/email_or_username_validator.hpp"

#include "forms/user/validators/email_validator.hpp"
#include "forms/user/validators/username_validator.hpp"

namespace user_forms
{

    form::ValidationResult EmailOrUsernameValidator::Validate(
        const std::string &value, const form::FormContext &context) const
    {
        if (value.empty())
        {
            return form::ValidationResult::Invalid("Email or username cannot be empty");
        }

        if (value.contains('@'))
        {
            EmailValidator email_validator;
            return email_validator.Validate(value, context);
        }
        else
        {
            UsernameValidator username_validator;
            return username_validator.Validate(value, context);
        }
    }

    std::optional<std::string> EmailOrUsernameValidator::GetHint() const
    {
        return "Enter a valid email address or username (3-20 alphanumeric characters or underscores)";
    }

} // namespace user_forms
