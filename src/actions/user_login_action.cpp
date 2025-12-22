#include "actions/user_login_action.hpp"
#include "forms/user/login_form.hpp"

void UserLoginAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== User Login ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::UserLogin data;
  user_forms::LoginForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled) {
    context.output->WriteLine("");
    context.output->WriteLine("Login cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void UserLoginAction::DisplayResults(const dto::UserLogin& data,
                                     ActionContext& context) {
  context.output->WriteLine("");
  context.output->WriteLine("=== Login Successful ===");
  context.output->WriteLine("Username: " + data.username);
  context.output->WriteLine("Welcome back!");
}
