#include "forms/user/registration_form.hpp"
#include "forms/user/fields/email_field.hpp"
#include "forms/user/fields/full_name_field.hpp"
#include "forms/user/fields/password_field.hpp"
#include "core/ui/form/form_builder_utils.hpp"
namespace user_forms
{
  RegistrationForm::RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                     std::shared_ptr<Output> output)
      : form::Form(SetupFormLayout(), input_provider, output) {}
  std::vector<std::shared_ptr<form::Field>> RegistrationForm::SetupFormLayout()
  {
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<FullNameField>(fields);
    form::AddField<EmailField>(fields);
    form::AddField<PasswordField>(fields);
    return fields;
  }
}