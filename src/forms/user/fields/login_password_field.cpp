#include "forms/user/fields/login_password_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_login.hpp"

namespace user_forms
{

LoginPasswordField::LoginPasswordField()
    : form::TextField(
          "password",
          "Enter password",
          form::SimpleFieldBinder<dto::UserLogin, std::string>(
              &dto::UserLogin::password)) {}

}  // namespace user_forms
