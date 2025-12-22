#include "forms/transaction/activity_summary_form.hpp"

#include "core/ui/form/data_source.hpp"
#include "dto/constants.hpp"
#include "forms/transaction/fields/end_date_field.hpp"
#include "forms/transaction/fields/start_date_field.hpp"
#include "forms/transaction/fields/timeframe_field.hpp"

namespace transaction_forms {

ActivitySummaryForm::ActivitySummaryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                         std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output) {}

std::vector<std::shared_ptr<form::Field>> ActivitySummaryForm::SetupFormLayout() {
  auto start_date_source = std::make_shared<form::ContextualDataSource>(
      [](const form::FormContext& form_context) -> std::vector<std::string> {
        auto timeframe = form_context.GetValue("timeframe");
        if (timeframe && *timeframe == dto::timeframe::MONTHLY) {
          return dto::sample_dates::GetMonthlySamples();
        } else if (timeframe && *timeframe == dto::timeframe::YEARLY) {
          return dto::sample_dates::GetYearlySamples();
        }
        return dto::sample_dates::GetDailySamples();
      });

  auto end_date_source = std::make_shared<form::ContextualDataSource>(
      [](const form::FormContext& form_context) -> std::vector<std::string> {
        auto timeframe = form_context.GetValue("timeframe");
        if (timeframe && *timeframe == dto::timeframe::MONTHLY) {
          return dto::sample_dates::GetMonthlySamples();
        } else if (timeframe && *timeframe == dto::timeframe::YEARLY) {
          return dto::sample_dates::GetYearlySamples();
        }
        return dto::sample_dates::GetDailySamples();
      });

  std::vector<std::shared_ptr<form::Field>> fields;
  fields.push_back(std::make_shared<TimeframeField>());
  fields.push_back(std::make_shared<StartDateField>(start_date_source));
  fields.push_back(std::make_shared<EndDateField>(end_date_source));
  return fields;
}

}  // namespace transaction_forms
