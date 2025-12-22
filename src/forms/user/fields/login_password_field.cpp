#include "forms/user/fields/login_password_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_login.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms {

LoginPasswordField::LoginPasswordField()
    : form::TextField(
          "password", "Enter password",
          form::SimpleFieldBinder<dto::UserLogin, std::string>(&dto::UserLogin::password),
          PasswordValidator()) {}

}  // namespace user_forms
