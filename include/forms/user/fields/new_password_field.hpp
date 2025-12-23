#pragma once
#include "forms/shared/fields/password_field.hpp"
#include "dto/user_reset.hpp"
namespace user_forms
{
  class NewPasswordField : public forms::shared::PasswordField<dto::UserReset>
  {
  public:
    explicit NewPasswordField();
  };
}