#pragma once
#include "forms/shared/fields/password_field.hpp"
#include "dto/user_registration.hpp"
namespace user_forms
{
  class PasswordField : public forms::shared::PasswordField<dto::UserRegistration>
  {
  public:
    explicit PasswordField();
  };
}