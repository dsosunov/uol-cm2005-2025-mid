#include "actions/user/user_reset_action.hpp"

#include "core/actions/action_helper.hpp"
#include "forms/user/reset_form.hpp"

#include <format>

UserResetAction::UserResetAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}
void UserResetAction::Execute(ActionContext& context)
{
    dto::UserReset data;
    user_forms::ResetForm form(context.form_input_provider, context.output);
    if (auto form_result = form.Read(data);
        actions::ActionHelper::HandleFormCancellation(form_result, "Reset", context))
    {
        return;
    }
    auto result = user_service_->ResetPassword(data.email_or_username, data.new_password);
    DisplayResults(result, data.email_or_username, context);
}
void UserResetAction::DisplayResults(const utils::ServiceResult<void>& result,
                                     const std::string& account, ActionContext& context) const
{
    actions::ActionHelper::DisplayResult(
        result.success, "Password Reset", result.message, context, [&context, &account]() {
            context.output->WriteLine(std::format("Account: {}", account));
            context.output->WriteLine("New password has been set.");
        });
}
