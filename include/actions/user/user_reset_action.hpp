#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/user_reset.hpp"
#include "forms/user/reset_form.hpp"
#include "services/user_service.hpp"

#include <memory>

class UserResetAction : public actions::FormBasedAction<user_forms::ResetForm, dto::UserReset,
                                                        utils::ServiceResult<void>>
{
  public:
    explicit UserResetAction(std::shared_ptr<services::UserService> user_service);

  protected:
    user_forms::ResetForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<void> ExecuteService(const dto::UserReset& data,
                                              ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<void>& result, const dto::UserReset& data,
                        ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Password Reset";
    }

  private:
    std::shared_ptr<services::UserService> user_service_;
};