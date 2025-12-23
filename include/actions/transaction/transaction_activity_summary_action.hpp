#pragma once
#include <memory>
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/activity_summary.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"
class TransactionActivitySummaryAction : public MenuAction
{
public:
  explicit TransactionActivitySummaryAction(
      std::shared_ptr<services::TransactionsService> transactions_service,
      std::shared_ptr<services::TradingService> trading_service);
  void Execute(ActionContext &context) override;
private:
  void DisplayResults(const dto::ActivitySummary &query, const services::ActivityStats &stats,
                      ActionContext &context) const;
  std::shared_ptr<services::TransactionsService> transactions_service_;
  std::shared_ptr<services::TradingService> trading_service_;
};