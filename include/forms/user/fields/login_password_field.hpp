#pragma once
#include "forms/shared/fields/password_field.hpp"
#include "dto/user_login.hpp"
namespace user_forms
{
  class LoginPasswordField : public forms::shared::PasswordField<dto::UserLogin>
  {
  public:
    explicit LoginPasswordField();
  };
}