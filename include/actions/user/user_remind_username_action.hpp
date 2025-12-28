#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/user_remind_username.hpp"
#include "forms/user/remind_username_form.hpp"
#include "services/user_service.hpp"

#include <memory>

class UserRemindUsernameAction
    : public actions::FormBasedAction<user_forms::RemindUsernameForm, dto::UserRemindUsername,
                                      utils::ServiceResult<std::string>>
{
  public:
    explicit UserRemindUsernameAction(std::shared_ptr<services::UserService> user_service);

  protected:
    user_forms::RemindUsernameForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<std::string> ExecuteService(const dto::UserRemindUsername& data,
                                                     ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<std::string>& result,
                        const dto::UserRemindUsername& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Remind Username";
    }

  private:
    std::shared_ptr<services::UserService> user_service_;
};
