#include "forms/user/registration_form.hpp"

#include "forms/user/fields/email_field.hpp"
#include "forms/user/fields/full_name_field.hpp"
#include "forms/user/fields/password_field.hpp"

namespace user_forms {

RegistrationForm::RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                   std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output) {}

std::vector<std::shared_ptr<form::Field>> RegistrationForm::SetupFormLayout() {
  std::vector<std::shared_ptr<form::Field>> fields;
  fields.push_back(std::make_shared<FullNameField>());
  fields.push_back(std::make_shared<EmailField>());
  fields.push_back(std::make_shared<PasswordField>());
  return fields;
}

}  // namespace user_forms
