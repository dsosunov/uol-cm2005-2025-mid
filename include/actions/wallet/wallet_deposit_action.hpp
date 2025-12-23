#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/wallet_operation.hpp"

class WalletDepositAction : public MenuAction
{
public:
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const dto::WalletOperation &data, ActionContext &context) const;
};
