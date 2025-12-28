#include "forms/user/fields/remind_email_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_remind_username.hpp"

namespace user_forms
{

RemindEmailField::RemindEmailField()
    : form::TextField("email", "Email",
                      form::SimpleFieldBinder<dto::UserRemindUsername, std::string>(
                          &dto::UserRemindUsername::email),
                      form::ValidatorFactory::Email())
{
}

} // namespace user_forms
