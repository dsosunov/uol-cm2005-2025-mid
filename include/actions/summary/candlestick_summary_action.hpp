#pragma once
#include "core/actions/action_context.hpp"
#include "core/actions/menu_action.hpp"
#include "dto/candlestick_query.hpp"

class CandlestickSummaryAction : public MenuAction
{
public:
  void Execute(ActionContext &context) override;

private:
  void DisplayResults(const dto::CandlestickQuery &query, ActionContext &context) const;
};
