#pragma once
#include "dto/user_login.hpp"
#include "forms/shared/fields/password_field.hpp"

namespace user_forms
{
class LoginPasswordField : public forms::shared::PasswordField<dto::UserLogin>
{
  public:
    explicit LoginPasswordField();
};
} // namespace user_forms