#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/transaction_query.hpp"

class TransactionShowByPairAction : public MenuAction
{
public:
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const dto::TransactionQuery &data, ActionContext &context) const;
};
