#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/activity_summary.hpp"

class TransactionActivitySummaryAction : public MenuAction
{
public:
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const dto::ActivitySummary &data, ActionContext &context) const;
};