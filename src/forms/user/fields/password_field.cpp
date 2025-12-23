#include "forms/user/fields/password_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_registration.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms
{

    PasswordField::PasswordField()
        : form::TextField("password", "Password",
                          form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                              &dto::UserRegistration::password),
                          std::make_shared<PasswordValidator>()) {}

} // namespace user_forms
