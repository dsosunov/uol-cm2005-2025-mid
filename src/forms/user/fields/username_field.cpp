#include "forms/user/fields/username_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_registration.hpp"

namespace user_forms
{

UsernameField::UsernameField(std::optional<std::string> default_username)
    : form::TextField("username", "Username",
                      form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                          &dto::UserRegistration::username),
                      std::move(default_username), form::ValidatorFactory::Username())
{
}

} // namespace user_forms
