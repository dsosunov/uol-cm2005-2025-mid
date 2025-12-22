#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"

class WalletViewBalanceAction : public MenuAction {
 public:
  void Execute(ActionContext& context) override;
};
