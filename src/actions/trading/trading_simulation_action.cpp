#include "actions/trading/trading_simulation_action.hpp"

#include "services/trading_activities_service.hpp"

TradingSimulationAction::TradingSimulationAction(
    std::shared_ptr<services::TradingActivitiesService> trading_activities_service)
    : trading_activities_service_(std::move(trading_activities_service))
{
    if (!trading_activities_service_)
    {
        throw std::invalid_argument("TradingActivitiesService is required");
    }
}

void TradingSimulationAction::Execute(ActionContext& context)
{
    auto result = trading_activities_service_->SimulateUserTradingActivities(5);
    if (!result.success)
    {
        DisplayFailureHeader(result.message, context);
        DisplayResultFooter(context);
        return;
    }

    DisplaySuccessHeader(context);
    if (result.data.has_value())
    {
        DisplayField("Product pairs", result.data->product_pairs, context);
        DisplayField("Orders created", result.data->orders_created, context);
        DisplayField("Orders per pair", result.data->orders_per_pair, context);
    }
    WriteLine(result.message, context);
    DisplayResultFooter(context);
}
