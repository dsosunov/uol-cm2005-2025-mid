#pragma once
#include <memory>

#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/candlestick_query.hpp"
#include "services/trading_service.hpp"

class CandlestickSummaryAction : public MenuAction
{
public:
  explicit CandlestickSummaryAction(std::shared_ptr<services::TradingService> trading_service);
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const dto::CandlestickQuery &query,
                      const services::CandlestickSummaryResult &result, ActionContext &context) const;

  std::shared_ptr<services::TradingService> trading_service_;
};
