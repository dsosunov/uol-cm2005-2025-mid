#include "forms/user/fields/email_or_username_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_reset.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms
{

    EmailOrUsernameField::EmailOrUsernameField()
        : form::TextField(
              "email_or_username", "Email or username",
              form::SimpleFieldBinder<dto::UserReset, std::string>(&dto::UserReset::email_or_username),
              std::make_shared<EmailOrUsernameValidator>()) {}

} // namespace user_forms
