#include "forms/user/fields/new_password_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_reset.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms {

NewPasswordField::NewPasswordField()
    : form::TextField(
          "new_password", "Enter new password",
          form::SimpleFieldBinder<dto::UserReset, std::string>(&dto::UserReset::new_password),
          PasswordValidator()) {}

}  // namespace user_forms
