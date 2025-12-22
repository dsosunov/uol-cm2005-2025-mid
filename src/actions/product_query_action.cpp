#include "actions/product_query_action.hpp"
#include "core/ui/form/form_builder.hpp"
#include "core/ui/form/data_source.hpp"
#include "forms/product_query/fields/currency_pair_field.hpp"
#include "forms/product_query/fields/query_type_field.hpp"
#include "forms/product_query/fields/timeframe_field.hpp"
#include "forms/product_query/fields/start_date_field.hpp"
#include "forms/product_query/fields/end_date_field.hpp"
#include <set>

static const std::set<std::string> kAllowedCurrencies = {
    "USD", "CAD", "EUR", "GBP", "JPY", "AUD", "CHF", "CNY"};

static std::unique_ptr<form::Form> CreateProductQueryForm(ActionContext& context) {
  // Create contextual data sources for date fields
  auto start_date_source = std::make_shared<form::ContextualDataSource>(
      [](const form::FormContext& form_context) -> std::vector<std::string> {
        auto timeframe = form_context.GetValue("timeframe");
        if (timeframe && *timeframe == "monthly") {
          return {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05",
                  "2025-06", "2025-07", "2025-08", "2025-09", "2025-10",
                  "2025-11", "2025-12"};
        } else if (timeframe && *timeframe == "yearly") {
          return {"2020", "2021", "2022", "2023", "2024", "2025"};
        }
        return {"2025-12-01", "2025-12-15", "2025-12-22"};
      });

  auto end_date_source = std::make_shared<form::ContextualDataSource>(
      [](const form::FormContext& form_context) -> std::vector<std::string> {
        auto timeframe = form_context.GetValue("timeframe");
        if (timeframe && *timeframe == "monthly") {
          return {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05",
                  "2025-06", "2025-07", "2025-08", "2025-09", "2025-10",
                  "2025-11", "2025-12"};
        } else if (timeframe && *timeframe == "yearly") {
          return {"2020", "2021", "2022", "2023", "2024", "2025"};
        }
        return {"2025-12-01", "2025-12-15", "2025-12-22"};
      });

  // Build form using FormBuilder
  form::FormBuilder builder(context.form_input_provider, context.output);

  builder.AddField(std::make_shared<product_query::CurrencyPairField>(kAllowedCurrencies));
  builder.AddField(std::make_shared<product_query::QueryTypeField>());
  builder.AddField(std::make_shared<product_query::TimeframeField>());

  builder.AddField(std::make_shared<product_query::StartDateField>(start_date_source));
  builder.AddField(std::make_shared<product_query::EndDateField>(end_date_source));

  return builder.Build();
}

void ProductQueryAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== Product Query ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::ProductQuery query;
  auto form = CreateProductQueryForm(context);

  form::FormReadResult result = form->Read(query);

  if (result == form::FormReadResult::kCancelled) {
    context.output->WriteLine("");
    context.output->WriteLine("Query cancelled by user.");
    return;
  }

  DisplayResults(query, context);
}

void ProductQueryAction::DisplayResults(const dto::ProductQuery& query,
                                        ActionContext& context) {
  context.output->WriteLine("");
  context.output->WriteLine("=== Query Results ===");
  context.output->WriteLine("Product Pair: " + query.currency_base + "/" +
                            query.currency_quote);
  context.output->WriteLine("Query Type: " + query.query_type);
  context.output->WriteLine("Timeframe: " + query.timeframe);

  if (!query.start_date.empty()) {
    context.output->WriteLine("Start Date: " + query.start_date);
  }

  if (!query.end_date.empty()) {
    context.output->WriteLine("End Date: " + query.end_date);
  }
}
