#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "services/trading_service.hpp"

#include <memory>

class AutoGenerateTradesAction : public MenuAction
{
  public:
    explicit AutoGenerateTradesAction(std::shared_ptr<services::TradingService> trading_service);
    void Execute(ActionContext& context) override;

  private:
    std::shared_ptr<services::TradingService> trading_service_;
};