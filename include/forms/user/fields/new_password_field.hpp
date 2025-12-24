#pragma once
#include "dto/user_reset.hpp"
#include "forms/shared/fields/password_field.hpp"

namespace user_forms
{
class NewPasswordField : public forms::shared::PasswordField<dto::UserReset>
{
  public:
    explicit NewPasswordField();
};
} // namespace user_forms