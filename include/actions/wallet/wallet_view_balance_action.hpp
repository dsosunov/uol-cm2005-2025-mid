#pragma once
#include <memory>
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "services/wallet_service.hpp"
class WalletViewBalanceAction : public MenuAction
{
public:
  explicit WalletViewBalanceAction(std::shared_ptr<services::WalletService> wallet_service);
  void Execute(ActionContext &context) override;
private:
  std::shared_ptr<services::WalletService> wallet_service_;
};