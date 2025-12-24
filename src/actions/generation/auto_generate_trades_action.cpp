#include "actions/generation/auto_generate_trades_action.hpp"

#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/utils/output_formatter.hpp"

#include <format>

AutoGenerateTradesAction::AutoGenerateTradesAction(
    std::shared_ptr<services::TradingService> trading_service)
    : trading_service_(std::move(trading_service))
{
}
void AutoGenerateTradesAction::Execute(ActionContext& context)
{
    context.output->WriteLine("");
    context.output->WriteLine("This action will automatically generate sample trades");
    context.output->WriteLine("based on current market conditions.");
    context.output->WriteLine("");
    context.output->WriteLine("Press Enter to confirm, or type 'cancel' to abort...");
    form::TextField confirmField(
        "confirm", "Confirm",
        [](const std::any&, const std::string&,
           const form::FormContext&) { /* not implemented yet */ },
        nullptr);
    form::FormContext emptyContext;
    if (auto inputOpt = context.form_input_provider->ReadField(confirmField, emptyContext);
        !inputOpt || *inputOpt == "cancel")
    {
        context.output->WriteLine("");
        context.output->WriteLine("Auto-generation cancelled.");
        return;
    }
    auto result = trading_service_->GenerateTrades(10);
    context.output->WriteLine("");
    if (result.success)
    {
        context.output->WriteLine(utils::OutputFormatter::SuccessHeader("Trade Generation"));
        context.output->WriteLine(
            std::format("Generated {} trades successfully:", result.data->trades_generated));
        for (const auto& [pair, count] : result.data->trades_by_pair)
        {
            if (count > 0)
            {
                context.output->WriteLine(std::format("  - {} x {}", count, pair));
            }
        }
        context.output->WriteLine("");
        context.output->WriteLine(std::format("Total volume: ${:.2f}", result.data->total_volume));
    }
    else
    {
        context.output->WriteLine(utils::OutputFormatter::FailureHeader("Trade Generation"));
        context.output->WriteLine(std::format("Error: {}", result.message));
    }
}
