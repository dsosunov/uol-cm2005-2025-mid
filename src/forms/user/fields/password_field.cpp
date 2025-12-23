#include "forms/user/fields/password_field.hpp"
#include "dto/user_registration.hpp"
#include "forms/shared/fields/password_field.hpp"
namespace user_forms
{
    PasswordField::PasswordField()
        : forms::shared::PasswordField<dto::UserRegistration>(
              "password", "Password", &dto::UserRegistration::password) {}
}