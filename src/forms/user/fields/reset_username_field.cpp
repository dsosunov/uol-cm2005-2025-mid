#include "forms/user/fields/reset_username_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_reset.hpp"

namespace user_forms
{

ResetUsernameField::ResetUsernameField()
    : form::TextField(
          "username", "Username",
          form::SimpleFieldBinder<dto::UserReset, std::string>(&dto::UserReset::username),
          form::ValidatorFactory::Username())
{
}

} // namespace user_forms
