#include "actions/user/user_login_action.hpp"

#include <format>

#include "forms/user/login_form.hpp"

UserLoginAction::UserLoginAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

void UserLoginAction::Execute(ActionContext &context)
{
  dto::UserLogin data;
  user_forms::LoginForm form(context.form_input_provider, context.output);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Login cancelled by user.");
    return;
  }
  auto result = user_service_->LoginUser(data.username, data.password);
  DisplayResults(result, context);
}

void UserLoginAction::DisplayResults(const services::LoginResult &result,
                                     ActionContext &context) const
{
  context.output->WriteLine("");
  if (result.success)
  {
    context.output->WriteLine("=== Login Successful ===");
    if (result.user.has_value())
    {
      context.output->WriteLine(std::format("Welcome back, {}!", result.user->full_name));
      context.output->WriteLine(std::format("Username: {}", result.user->username));
    }
  }
  else
  {
    context.output->WriteLine("=== Login Failed ===");
    context.output->WriteLine(std::format("Error: {}", result.message));
  }
}
