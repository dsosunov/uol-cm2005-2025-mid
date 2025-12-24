#include "actions/user/user_register_action.hpp"

#include <format>

UserRegisterAction::UserRegisterAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

user_forms::RegistrationForm UserRegisterAction::CreateForm(ActionContext& context)
{
    return user_forms::RegistrationForm(context.form_input_provider, context.output);
}
utils::ServiceResult<services::User> UserRegisterAction::ExecuteService(
    const dto::UserRegistration& data, ActionContext& context)
{
    return user_service_->RegisterUser(data.full_name, data.email, data.password);
}
void UserRegisterAction::DisplayResults(const utils::ServiceResult<services::User>& result,
                                        const dto::UserRegistration& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        if (result.data.has_value())
        {
            DisplayField("Full Name", result.data->full_name, context);
            DisplayField("Email", result.data->email, context);
        }
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
