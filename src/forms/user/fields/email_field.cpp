#include "forms/user/fields/email_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_registration.hpp"

namespace user_forms
{

EmailField::EmailField()
    : form::TextField(
          "email",
          "Enter email address",
          form::SimpleFieldBinder<dto::UserRegistration, std::string>(
              &dto::UserRegistration::email)) {}

}  // namespace user_forms
