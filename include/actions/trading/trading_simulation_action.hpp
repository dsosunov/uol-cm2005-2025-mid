#pragma once

#include "core/actions/menu_action.hpp"

#include <memory>

namespace services
{
class TradingActivitiesService;
}

class TradingSimulationAction : public MenuAction
{
  public:
    explicit TradingSimulationAction(
        std::shared_ptr<services::TradingActivitiesService> trading_activities_service);

    void Execute(ActionContext& context) override;

  protected:
    const char* GetOperationName() const override
    {
        return "Trading";
    }

  private:
    std::shared_ptr<services::TradingActivitiesService> trading_activities_service_;
};
