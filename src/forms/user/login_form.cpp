#include "forms/user/login_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/user/fields/login_password_field.hpp"
#include "forms/user/fields/username_field.hpp"

namespace user_forms
{
LoginForm::LoginForm(std::shared_ptr<form::FormInputProvider> input_provider,
                     std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> LoginForm::SetupFormLayout()
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<UsernameField>(fields);
    form::AddField<LoginPasswordField>(fields);

    return fields;
}
} // namespace user_forms