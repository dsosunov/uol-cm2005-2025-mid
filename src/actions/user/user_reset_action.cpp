#include "actions/user/user_reset_action.hpp"

#include <format>

#include "forms/user/reset_form.hpp"

UserResetAction::UserResetAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

void UserResetAction::Execute(ActionContext &context)
{
  dto::UserReset data;
  user_forms::ResetForm form(context.form_input_provider, context.output);

  if (auto result = form.Read(data); result == form::FormReadResult::kCancelled)
  {
    context.output->WriteLine("");
    context.output->WriteLine("Reset cancelled by user.");
    return;
  }
  auto result = user_service_->ResetPassword(data.email_or_username, data.new_password);
  DisplayResults(result, data.email_or_username, context);
}

void UserResetAction::DisplayResults(const services::ResetResult &result,
                                     const std::string &account, ActionContext &context) const
{
  context.output->WriteLine("");
  if (result.success)
  {
    context.output->WriteLine("=== Password Reset Successful ===");
    context.output->WriteLine(std::format("Account: {}", account));
    context.output->WriteLine("New password has been set.");
  }
  else
  {
    context.output->WriteLine("=== Password Reset Failed ===");
    context.output->WriteLine(std::format("Error: {}", result.message));
  }
}
