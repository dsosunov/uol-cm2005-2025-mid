#include "forms/user/fields/login_password_field.hpp"

#include "dto/user_login.hpp"
#include "forms/shared/fields/password_field.hpp"

namespace user_forms
{

LoginPasswordField::LoginPasswordField()
    : forms::shared::PasswordField<dto::UserLogin>("password", "Password",
                                                   &dto::UserLogin::password)
{
}

} // namespace user_forms