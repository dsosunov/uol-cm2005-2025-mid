#pragma once
#include <memory>

#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/user_registration.hpp"
#include "services/user_service.hpp"

class UserRegisterAction : public MenuAction
{
public:
  explicit UserRegisterAction(std::shared_ptr<services::UserService> user_service);
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const services::RegistrationResult &result, ActionContext &context) const;

  std::shared_ptr<services::UserService> user_service_;
};
