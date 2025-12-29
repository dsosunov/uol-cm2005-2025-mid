#pragma once
#include "core/actions/menu_action.hpp"

class TradingStubAction : public MenuAction
{
  public:
    void Execute(ActionContext& context) override;

  protected:
    const char* GetOperationName() const override
    {
        return "Trading";
    }
};
