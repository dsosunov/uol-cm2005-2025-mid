#pragma once
#include <memory>

#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/wallet_operation.hpp"
#include "services/wallet_service.hpp"
#include "services/trading_service.hpp"

class WalletDepositAction : public MenuAction
{
public:
  explicit WalletDepositAction(std::shared_ptr<services::WalletService> wallet_service,
                               std::shared_ptr<services::TradingService> trading_service);
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const services::OperationResult &result, const std::string &currency,
                      ActionContext &context) const;

  std::shared_ptr<services::WalletService> wallet_service_;
  std::shared_ptr<services::TradingService> trading_service_;
};
