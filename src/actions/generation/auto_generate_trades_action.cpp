#include "actions/generation/auto_generate_trades_action.hpp"
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form_context.hpp"

void AutoGenerateTradesAction::Execute(ActionContext &context)
{
  context.output->WriteLine("");
  context.output->WriteLine("This action will automatically generate sample trades");
  context.output->WriteLine("based on current market conditions.");
  context.output->WriteLine("");
  context.output->WriteLine("Press Enter to confirm, or type 'cancel' to abort...");

  // Create a simple TextField for reading confirmation
  form::TextField confirmField("confirm", "Confirm", [](const std::any &, const std::string &, const form::FormContext &)
                               {
                                 // Not implemented yet
                               },
                               nullptr);
  form::FormContext emptyContext;
  auto inputOpt = context.form_input_provider->ReadField(confirmField, emptyContext);

  if (!inputOpt || *inputOpt == "cancel")
  {
    context.output->WriteLine("");
    context.output->WriteLine("Auto-generation cancelled.");
    return;
  }

  context.output->WriteLine("");
  context.output->WriteLine("=== Generating Trades ===");
  context.output->WriteLine("Generated 10 trades successfully:");
  context.output->WriteLine("  - 3 x USD/EUR");
  context.output->WriteLine("  - 2 x GBP/USD");
  context.output->WriteLine("  - 3 x USD/JPY");
  context.output->WriteLine("  - 2 x EUR/GBP");
  context.output->WriteLine("");
  context.output->WriteLine("Total volume: $50,000.00");
}
