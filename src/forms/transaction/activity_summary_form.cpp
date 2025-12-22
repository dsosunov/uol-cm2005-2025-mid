#include "forms/transaction/activity_summary_form.hpp"
#include "core/ui/form/data_source.hpp"
#include "forms/transaction/fields/timeframe_field.hpp"
#include "forms/transaction/fields/start_date_field.hpp"
#include "forms/transaction/fields/end_date_field.hpp"

namespace transaction_forms {

ActivitySummaryForm::ActivitySummaryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                         std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output) {}

std::vector<std::shared_ptr<form::Field>> ActivitySummaryForm::SetupFormLayout() {
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

  std::vector<std::shared_ptr<form::Field>> fields;
  fields.push_back(std::make_shared<TimeframeField>());
  fields.push_back(std::make_shared<StartDateField>(start_date_source));
  fields.push_back(std::make_shared<EndDateField>(end_date_source));
  return fields;
}

}  // namespace transaction_forms
