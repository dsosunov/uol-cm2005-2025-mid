#include "actions/user/user_register_action.hpp"

#include "forms/user/registration_form.hpp"

void UserRegisterAction::Execute(ActionContext &context)
{
  context.output->WriteLine("=== User Registration ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::UserRegistration data;
  user_forms::RegistrationForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Registration cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void UserRegisterAction::DisplayResults(const dto::UserRegistration &data, ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Registration Successful ===");
  context.output->WriteLine("Full Name: " + data.full_name);
  context.output->WriteLine("Email: " + data.email);
  context.output->WriteLine("Password: " + std::string(data.password.length(), '*'));
}
