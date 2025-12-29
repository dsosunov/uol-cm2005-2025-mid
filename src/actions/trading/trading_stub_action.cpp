#include "actions/trading/trading_stub_action.hpp"

void TradingStubAction::Execute(ActionContext& context)
{
    DisplayFailureHeader("Trading feature is currently unavailable", context);
}
