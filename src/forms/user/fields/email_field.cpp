#include "forms/user/fields/email_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_registration.hpp"

namespace user_forms
{

EmailField::EmailField()
    : form::TextField("email", "Email",
                      form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                          &dto::UserRegistration::email),
                      form::ValidatorFactory::Email())
{
}

} // namespace user_forms