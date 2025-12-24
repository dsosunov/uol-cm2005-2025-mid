#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/user_login.hpp"
#include "forms/user/login_form.hpp"
#include "services/user_service.hpp"

#include <memory>

class UserLoginAction : public actions::FormBasedAction<user_forms::LoginForm, dto::UserLogin,
                                                        utils::ServiceResult<services::User>>
{
  public:
    explicit UserLoginAction(std::shared_ptr<services::UserService> user_service);

  protected:
    user_forms::LoginForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<services::User> ExecuteService(const dto::UserLogin& data,
                                                        ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<services::User>& result,
                        const dto::UserLogin& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Login";
    }

  private:
    std::shared_ptr<services::UserService> user_service_;
};