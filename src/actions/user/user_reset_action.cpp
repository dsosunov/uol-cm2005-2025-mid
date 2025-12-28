#include "actions/user/user_reset_action.hpp"

#include <format>

UserResetAction::UserResetAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

user_forms::ResetForm UserResetAction::CreateForm(ActionContext& context)
{
    return user_forms::ResetForm(context.form_input_provider, context.output);
}

utils::ServiceResult<void> UserResetAction::ExecuteService(const dto::UserReset& data,
                                                           ActionContext& context)
{
    return user_service_->ResetPassword(data.username, data.new_password);
}

void UserResetAction::DisplayResults(const utils::ServiceResult<void>& result,
                                     const dto::UserReset& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        DisplayField("Account", data.username, context);
        WriteLine("New password has been set.", context);
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
