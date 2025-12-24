#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/transaction_query.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"

#include <memory>

class TransactionShowByPairAction : public MenuAction
{
  public:
    explicit TransactionShowByPairAction(
        std::shared_ptr<services::TransactionsService> transactions_service,
        std::shared_ptr<services::TradingService> trading_service);
    void Execute(ActionContext& context) override;

  private:
    void DisplayResults(const std::string& product_pair,
                        const std::vector<services::Transaction>& transactions,
                        ActionContext& context) const;
    std::shared_ptr<services::TransactionsService> transactions_service_;
    std::shared_ptr<services::TradingService> trading_service_;
};