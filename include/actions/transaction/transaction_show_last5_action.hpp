#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"

class TransactionShowLast5Action : public MenuAction {
 public:
  void Execute(ActionContext& context) override;
};
