#include "actions/user/user_login_action.hpp"

#include "core/actions/action_helper.hpp"

#include <format>

UserLoginAction::UserLoginAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

user_forms::LoginForm UserLoginAction::CreateForm(ActionContext& context)
{
    return user_forms::LoginForm(context.form_input_provider, context.output);
}
utils::ServiceResult<services::User> UserLoginAction::ExecuteService(const dto::UserLogin& data,
                                                                     ActionContext& context)
{
    return user_service_->LoginUser(data.username, data.password);
}
void UserLoginAction::DisplayResults(const utils::ServiceResult<services::User>& result,
                                     const dto::UserLogin& data, ActionContext& context)
{
    actions::ActionHelper::DisplayResult(
        result.success, GetOperationName(), result.message, context, [&result, &context]() {
            if (result.data.has_value())
            {
                context.output->WriteLine(std::format("Welcome back, {}!", result.data->full_name));
                context.output->WriteLine(std::format("Username: {}", result.data->username));
            }
        });
}
