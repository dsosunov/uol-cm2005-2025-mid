#include "forms/user/registration_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/user/fields/email_field.hpp"
#include "forms/user/fields/full_name_field.hpp"
#include "forms/user/fields/password_field.hpp"
#include "forms/user/fields/username_field.hpp"

namespace user_forms
{
RegistrationForm::RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                   std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(std::nullopt), input_provider, output)
{
}

RegistrationForm::RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                   std::shared_ptr<Output> output,
                                   std::optional<std::string> default_username)
    : form::Form(SetupFormLayout(default_username), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> RegistrationForm::SetupFormLayout(
    const std::optional<std::string>& default_username)
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<UsernameField>(fields, default_username);
    form::AddField<FullNameField>(fields);
    form::AddField<EmailField>(fields);
    form::AddField<PasswordField>(fields);

    return fields;
}
} // namespace user_forms