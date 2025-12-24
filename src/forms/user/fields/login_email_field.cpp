#include "forms/user/fields/login_email_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_login.hpp"

namespace user_forms
{

LoginEmailField::LoginEmailField()
    : form::TextField("email", "Email",
                      form::SimpleFieldBinder<dto::UserLogin, std::string>(&dto::UserLogin::email),
                      form::ValidatorFactory::Email())
{
}

} // namespace user_forms
