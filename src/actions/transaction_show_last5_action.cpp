#include "actions/transaction_show_last5_action.hpp"

void TransactionShowLast5Action::Execute(ActionContext& context) {
  context.output->WriteLine("=== Last 5 Transactions ===");
  context.output->WriteLine("");
  context.output->WriteLine("1. USD/EUR - Buy  - 100.00 @ 0.95 - 2025-12-22 10:30");
  context.output->WriteLine("2. GBP/USD - Sell - 50.00  @ 1.28 - 2025-12-22 09:15");
  context.output->WriteLine("3. USD/JPY - Buy  - 200.00 @ 150.5 - 2025-12-21 15:45");
  context.output->WriteLine("4. EUR/GBP - Sell - 75.00  @ 0.85 - 2025-12-21 12:20");
  context.output->WriteLine("5. CAD/USD - Buy  - 150.00 @ 0.73 - 2025-12-20 16:00");
}
