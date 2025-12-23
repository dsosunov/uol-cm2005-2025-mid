#include "forms/user/fields/new_password_field.hpp"
#include "dto/user_reset.hpp"
#include "forms/shared/fields/password_field.hpp"
namespace user_forms
{
    NewPasswordField::NewPasswordField()
        : forms::shared::PasswordField<dto::UserReset>(
              "new_password", "New password", &dto::UserReset::new_password) {}
}