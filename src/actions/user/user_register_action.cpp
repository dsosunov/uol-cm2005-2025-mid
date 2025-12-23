#include "actions/user/user_register_action.hpp"

#include <format>

#include "forms/user/registration_form.hpp"

void UserRegisterAction::Execute(ActionContext &context)
{
  dto::UserRegistration data;
  user_forms::RegistrationForm form(context.form_input_provider, context.output);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Registration cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void UserRegisterAction::DisplayResults(const dto::UserRegistration &data, ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Registration Successful ===");
  context.output->WriteLine(std::format("Full Name: {}", data.full_name));
  context.output->WriteLine(std::format("Email: {}", data.email));
  context.output->WriteLine(std::format("Password: {}", std::string(data.password.length(), '*')));
}
