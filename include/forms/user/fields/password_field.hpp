#pragma once
#include "dto/user_registration.hpp"
#include "forms/shared/fields/password_field.hpp"

namespace user_forms
{
class PasswordField : public forms::shared::PasswordField<dto::UserRegistration>
{
  public:
    explicit PasswordField();
};
} // namespace user_forms