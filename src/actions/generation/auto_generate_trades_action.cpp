#include "actions/generation/auto_generate_trades_action.hpp"

#include <format>

AutoGenerateTradesAction::AutoGenerateTradesAction(std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}

generation_forms::GenerationConfirmationForm AutoGenerateTradesAction::CreateForm(ActionContext& context)
{
    return generation_forms::GenerationConfirmationForm(context.form_input_provider, context.output);
}

utils::ServiceResult<services::GenerationData> AutoGenerateTradesAction::ExecuteService(const dto::GenerationRequest& data, ActionContext& context)
{
    return trading_service_->GenerateTrades(10);
}

void AutoGenerateTradesAction::DisplayResults(const utils::ServiceResult<services::GenerationData>& result, const dto::GenerationRequest& data,
                                              ActionContext& context)
{
    if (result.success && result.data.has_value())
    {
        DisplaySuccessHeader(context);
        WriteLine(std::format("Generated {} trades successfully:", result.data->trades_generated), context);
        for (const auto& [pair, count] : result.data->trades_by_pair)
        {
            if (count > 0)
            {
                WriteLine(std::format("  - {} x {}", count, pair), context);
            }
        }
        WriteEmptyLine(context);
        WriteLine(std::format("Total volume: ${:.2f}", result.data->total_volume), context);
    }
    else
    {
        DisplayFailureHeader(result.message, context);
    }
}
