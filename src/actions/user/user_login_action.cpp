#include "actions/user/user_login_action.hpp"

#include <format>

#include "forms/user/login_form.hpp"

void UserLoginAction::Execute(ActionContext &context)
{
  dto::UserLogin data;
  user_forms::LoginForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Login cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void UserLoginAction::DisplayResults(const dto::UserLogin &data, ActionContext &context) const
{
  context.output->WriteLine("");
  context.output->WriteLine("=== Login Successful ===");
  context.output->WriteLine(std::format("Username: {}", data.username));
  context.output->WriteLine("Welcome back!");
}
