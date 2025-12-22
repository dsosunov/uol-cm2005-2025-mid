#include "actions/auto_generate_trades_action.hpp"

void AutoGenerateTradesAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== Auto-Generate Trades ===");
  context.output->WriteLine("");
  context.output->WriteLine("This action will automatically generate sample trades");
  context.output->WriteLine("based on current market conditions.");
  context.output->WriteLine("");
  context.output->WriteLine("Press Enter to confirm, or type 'cancel' to abort...");

  auto input = context.form_input_provider->ReadText("Confirm");

  if (input == "cancel") {
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
