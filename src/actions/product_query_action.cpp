#include "actions/product_query_action.hpp"
#include "core/ui/form/data_source.hpp"

ProductQueryAction::ProductQueryAction(std::shared_ptr<Input> input,
                                       std::shared_ptr<Output> output,
                                       MenuRenderer& renderer,
                                       MenuInput& menu_input)
    : input_(input),
      output_(output),
      form_(input, output, renderer, menu_input,
            std::make_shared<form::ContextualDataSource>(
                [](const form::FormContext& context) -> std::vector<std::string> {
                  auto timeframe = context.GetValue("timeframe");
                  if (timeframe && *timeframe == "monthly") {
                    return {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05",
                            "2025-06", "2025-07", "2025-08", "2025-09", "2025-10",
                            "2025-11", "2025-12"};
                  } else if (timeframe && *timeframe == "yearly") {
                    return {"2020", "2021", "2022", "2023", "2024", "2025"};
                  }
                  return {"2025-12-01", "2025-12-15", "2025-12-22"};
                }),
            std::make_shared<form::ContextualDataSource>(
                [](const form::FormContext& context) -> std::vector<std::string> {
                  auto timeframe = context.GetValue("timeframe");
                  if (timeframe && *timeframe == "monthly") {
                    return {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05",
                            "2025-06", "2025-07", "2025-08", "2025-09", "2025-10",
                            "2025-11", "2025-12"};
                  } else if (timeframe && *timeframe == "yearly") {
                    return {"2020", "2021", "2022", "2023", "2024", "2025"};
                  }
                  return {"2025-12-01", "2025-12-15", "2025-12-22"};
                })) {}

void ProductQueryAction::Execute() {
  output_->WriteLine("=== Product Query ===");
  output_->WriteLine("Type 'cancel' to abort at any time");
  output_->WriteLine("");

  dto::ProductQuery query;

  form::FormReadResult result = form_.Read(query);

  if (result == form::FormReadResult::kCancelled) {
    output_->WriteLine("");
    output_->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(query);
}

void ProductQueryAction::DisplayResults(const dto::ProductQuery& query) {
  output_->WriteLine("");
  output_->WriteLine("=== Query Results ===");
  output_->WriteLine("Product Pair: " + query.currency_base + "/" +
                     query.currency_quote);
  output_->WriteLine("Query Type: " + query.query_type);
  output_->WriteLine("Timeframe: " + query.timeframe);

  if (!query.start_date.empty()) {
    output_->WriteLine("Start Date: " + query.start_date);
  }

  if (!query.end_date.empty()) {
    output_->WriteLine("End Date: " + query.end_date);
  }
}
