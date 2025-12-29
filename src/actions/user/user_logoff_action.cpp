#include "actions/user/user_logoff_action.hpp"

UserLogoffAction::UserLogoffAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

shared_forms::EmptyForm UserLogoffAction::CreateForm(ActionContext& context)
{
    (void)context;
    return shared_forms::EmptyForm{};
}

utils::ServiceResult<void> UserLogoffAction::ExecuteService(const EmptyRequest& data,
                                                            ActionContext& context)
{
    (void)data;
    if (!context.auth_service->IsAuthenticated())
    {
        return utils::ServiceResult<void>::Success("Already logged off");
    }

    return user_service_->Logout();
}

void UserLogoffAction::DisplayResults(const utils::ServiceResult<void>& result,
                                      const EmptyRequest& data, ActionContext& context)
{
    (void)data;
    if (result.success)
    {
        DisplaySuccessHeader(context);
        WriteLine(result.message, context);
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
