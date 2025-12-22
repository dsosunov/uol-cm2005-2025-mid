#include "actions/user_reset_action.hpp"
#include "forms/user/reset_form.hpp"

void UserResetAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== Reset / Remind Password ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::UserReset data;
  user_forms::ResetForm form(context.form_input_provider, context.output);

  form::FormReadResult result = form.Read(data);

  if (result == form::FormReadResult::kCancelled) {
    context.output->WriteLine("");
    context.output->WriteLine("Reset cancelled by user.");
    return;
  }

  DisplayResults(data, context);
}

void UserResetAction::DisplayResults(const dto::UserReset& data,
                                     ActionContext& context) {
  context.output->WriteLine("");
  context.output->WriteLine("=== Password Reset Successful ===");
  context.output->WriteLine("Account: " + data.email_or_username);
  context.output->WriteLine("New password has been set.");
}
