#include "forms/user/reset_form.hpp"

#include "forms/user/fields/email_or_username_field.hpp"
#include "forms/user/fields/new_password_field.hpp"

namespace user_forms {

ResetForm::ResetForm(std::shared_ptr<form::FormInputProvider> input_provider,
                     std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output) {}

std::vector<std::shared_ptr<form::Field>> ResetForm::SetupFormLayout() {
  std::vector<std::shared_ptr<form::Field>> fields;
  fields.push_back(std::make_shared<EmailOrUsernameField>());
  fields.push_back(std::make_shared<NewPasswordField>());
  return fields;
}

}
