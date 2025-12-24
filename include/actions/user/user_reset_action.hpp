#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "core/utils/service_result.hpp"
#include "dto/user_reset.hpp"
#include "services/user_service.hpp"

#include <memory>

class UserResetAction : public MenuAction
{
  public:
    explicit UserResetAction(std::shared_ptr<services::UserService> user_service);
    void Execute(ActionContext& context) override;

  private:
    void DisplayResults(const utils::ServiceResult<void>& result, const std::string& account,
                        ActionContext& context) const;
    std::shared_ptr<services::UserService> user_service_;
};