#include "forms/user/fields/username_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_login.hpp"

namespace user_forms
{

UsernameField::UsernameField()
    : form::TextField(
          "username",
          "Enter username",
          form::SimpleFieldBinder<dto::UserLogin, std::string>(
              &dto::UserLogin::username)) {}

}  // namespace user_forms
