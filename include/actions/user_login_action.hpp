#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/user_login.hpp"

class UserLoginAction : public MenuAction {
 public:
  void Execute(ActionContext& context) override;

 private:
  void DisplayResults(const dto::UserLogin& data, ActionContext& context);
};
