#include "forms/user/fields/reset_email_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_reset.hpp"

namespace user_forms
{

ResetEmailField::ResetEmailField()
    : form::TextField("email", "Email",
                      form::SimpleFieldBinder<dto::UserReset, std::string>(&dto::UserReset::email),
                      form::ValidatorFactory::Email())
{
}

} // namespace user_forms
