#include "forms/transaction/activity_summary_form.hpp"

#include "core/ui/form/contextual_data_source.hpp"
#include "forms/transaction/fields/end_date_field.hpp"
#include "forms/transaction/fields/start_date_field.hpp"
#include "forms/transaction/fields/timeframe_field.hpp"

namespace transaction_forms
{

  ActivitySummaryForm::ActivitySummaryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                           std::shared_ptr<Output> output,
                                           std::shared_ptr<ActivitySummaryFormDataProvider> data_provider)
      : form::Form(SetupFormLayout(std::move(data_provider)), input_provider, output) {}

  std::vector<std::shared_ptr<form::Field>> ActivitySummaryForm::SetupFormLayout(
      std::shared_ptr<ActivitySummaryFormDataProvider> data_provider)
  {
    auto start_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext &form_context)
        {
          auto timeframe = form_context.GetValue("timeframe");
          return data_provider->GetStartDates(timeframe.value_or(""));
        });
    auto end_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext &form_context)
        {
          auto timeframe = form_context.GetValue("timeframe");
          auto start_date = form_context.GetValue("start_date");
          return data_provider->GetEndDates(
              timeframe.value_or(""),
              start_date.value_or(""));
        });

    std::vector<std::shared_ptr<form::Field>> fields;
    fields.push_back(std::make_shared<TimeframeField>());
    fields.push_back(std::make_shared<StartDateField>(start_date_source));
    fields.push_back(std::make_shared<EndDateField>(end_date_source));
    return fields;
  }

}
