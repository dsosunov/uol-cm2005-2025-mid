#include "forms/user/fields/login_username_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_login.hpp"

namespace user_forms
{

LoginUsernameField::LoginUsernameField()
    : form::TextField(
          "username", "Username",
          form::SimpleFieldBinder<dto::UserLogin, std::string>(&dto::UserLogin::username),
          form::ValidatorFactory::Username())
{
}

} // namespace user_forms
