#include "actions/user/user_register_action.hpp"

#include <format>

UserRegisterAction::UserRegisterAction(std::shared_ptr<services::UserService> user_service)
    : user_service_(std::move(user_service))
{
}

user_forms::RegistrationForm UserRegisterAction::CreateForm(ActionContext& context)
{
    // Generate a suggested username that can be accepted by pressing Enter.
    auto suggested_username = user_service_->GenerateUsername();
    return user_forms::RegistrationForm(context.form_input_provider, context.output,
                                        suggested_username);
}
utils::ServiceResult<services::User> UserRegisterAction::ExecuteService(
    const dto::UserRegistration& data, ActionContext& context)
{
    return user_service_->RegisterUser(data.username, data.full_name, data.email, data.password);
}
void UserRegisterAction::DisplayResults(const utils::ServiceResult<services::User>& result,
                                        const dto::UserRegistration& data, ActionContext& context)
{
    if (result.success)
    {
        DisplaySuccessHeader(context);
        if (result.data.has_value())
        {
            DisplayField("Username", result.data->username, context);
            DisplayField("Full Name", result.data->full_name, context);
            DisplayField("Email", result.data->email, context);
            WriteLine("Use your username to log in later.", context);
        }
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
