#pragma once
#include <memory>
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "services/transactions_service.hpp"
class TransactionShowLast5Action : public MenuAction
{
public:
  explicit TransactionShowLast5Action(
      std::shared_ptr<services::TransactionsService> transactions_service);
  void Execute(ActionContext &context) override;
private:
  std::shared_ptr<services::TransactionsService> transactions_service_;
};