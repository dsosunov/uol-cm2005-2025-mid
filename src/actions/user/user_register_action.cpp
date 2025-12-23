#include "actions/user/user_register_action.hpp"

#include <format>

#include "forms/user/registration_form.hpp"

UserRegisterAction::UserRegisterAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

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

  // Use service to register user
  auto result = user_service_->RegisterUser(data.full_name, data.email, data.password);
  DisplayResults(result, context);
}

void UserRegisterAction::DisplayResults(const services::RegistrationResult &result,
                                        ActionContext &context) const
{
  context.output->WriteLine("");
  if (result.success)
  {
    context.output->WriteLine("=== Registration Successful ===");
    if (result.user.has_value())
    {
      context.output->WriteLine(std::format("Full Name: {}", result.user->full_name));
      context.output->WriteLine(std::format("Email: {}", result.user->email));
      context.output->WriteLine(std::format("Username: {}", result.user->username));
    }
  }
  else
  {
    context.output->WriteLine("=== Registration Failed ===");
    context.output->WriteLine(std::format("Error: {}", result.message));
  }
}
