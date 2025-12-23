#include "forms/user/fields/email_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_registration.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms
{

    EmailField::EmailField()
        : form::TextField("email", "Email",
                          form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                              &dto::UserRegistration::email),
                          std::make_shared<EmailValidator>()) {}

}
