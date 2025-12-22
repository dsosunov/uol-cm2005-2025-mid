#include "forms/user/login_form.hpp"

#include "forms/user/fields/login_password_field.hpp"
#include "forms/user/fields/username_field.hpp"

namespace user_forms {

LoginForm::LoginForm(std::shared_ptr<form::FormInputProvider> input_provider,
                     std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output) {}

std::vector<std::shared_ptr<form::Field>> LoginForm::SetupFormLayout() {
  std::vector<std::shared_ptr<form::Field>> fields;
  fields.push_back(std::make_shared<UsernameField>());
  fields.push_back(std::make_shared<LoginPasswordField>());
  return fields;
}

}  // namespace user_forms
