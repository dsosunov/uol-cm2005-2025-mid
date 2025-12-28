#include "actions/user/user_remind_username_action.hpp"

UserRemindUsernameAction::UserRemindUsernameAction(
    std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

user_forms::RemindUsernameForm UserRemindUsernameAction::CreateForm(ActionContext& context)
{
    return user_forms::RemindUsernameForm(context.form_input_provider, context.output);
}

utils::ServiceResult<std::string> UserRemindUsernameAction::ExecuteService(
    const dto::UserRemindUsername& data, ActionContext& context)
{
    return user_service_->RemindUsername(data.email);
}

void UserRemindUsernameAction::DisplayResults(const utils::ServiceResult<std::string>& result,
                                             const dto::UserRemindUsername& data,
                                             ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        DisplayField("Email", data.email, context);
        if (result.data.has_value())
        {
            DisplayField("Username", result.data.value(), context);
        }
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
