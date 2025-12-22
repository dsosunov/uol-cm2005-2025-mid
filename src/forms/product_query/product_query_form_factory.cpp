#include "forms/product_query/product_query_form_factory.hpp"
#include "core/ui/form/data_source.hpp"
#include "forms/product_query/fields/currency_pair_field.hpp"
#include "forms/product_query/fields/query_type_field.hpp"
#include "forms/product_query/fields/timeframe_field.hpp"
#include "forms/product_query/fields/start_date_field.hpp"
#include "forms/product_query/fields/end_date_field.hpp"

namespace product_query {

ProductQueryFormFactory::ProductQueryFormFactory(const std::set<std::string>& allowed_currencies)
    : allowed_currencies_(allowed_currencies) {}

std::unique_ptr<form::Form> ProductQueryFormFactory::CreateForm(ActionContext& context) {
  form::FormBuilder builder(context.form_input_provider, context.output);
  SetupFormLayout(builder);
  return builder.Build();
}

void ProductQueryFormFactory::SetupFormLayout(form::FormBuilder& builder) {
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

  // Setup form field layout
  builder.AddField(std::make_shared<CurrencyPairField>(allowed_currencies_));
  builder.AddField(std::make_shared<QueryTypeField>());
  builder.AddField(std::make_shared<TimeframeField>());
  builder.AddField(std::make_shared<StartDateField>(start_date_source));
  builder.AddField(std::make_shared<EndDateField>(end_date_source));
}

}  // namespace product_query
