#include "forms/transaction/activity_summary_form.hpp"

#include "core/ui/form/contextual_data_source.hpp"
#include "core/ui/form/form_builder_utils.hpp"
#include "forms/transaction/fields/end_date_field.hpp"
#include "forms/transaction/fields/start_date_field.hpp"
#include "forms/transaction/fields/timeframe_field.hpp"

namespace transaction_forms
{
ActivitySummaryForm::ActivitySummaryForm(
    std::shared_ptr<form::FormInputProvider> input_provider, std::shared_ptr<Output> output,
    std::shared_ptr<ActivitySummaryFormDataProvider> data_provider)
    : form::Form(SetupFormLayout(std::move(data_provider)), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> ActivitySummaryForm::SetupFormLayout(
    std::shared_ptr<ActivitySummaryFormDataProvider> data_provider)
{
    auto start_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext& form_context) {
            if (auto timeframe = form_context.GetValue<dto::Timeframe>("timeframe"))
            {
                return data_provider->GetStartDates(*timeframe);
            }
            return std::vector<form::DataSource::OptionPair>{};
        });
    auto end_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext& form_context) {
            if (auto timeframe = form_context.GetValue<dto::Timeframe>("timeframe"))
            {
                auto start_date =
                    form_context.GetValue<std::optional<utils::TimePoint>>("start_date");
                return data_provider->GetEndDates(*timeframe, start_date.value_or(std::nullopt));
            }
            return std::vector<form::DataSource::OptionPair>{};
        });
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<TimeframeField>(fields);
    form::AddField<StartDateField>(fields, start_date_source);
    form::AddField<EndDateField>(fields, end_date_source);
    return fields;
}
} // namespace transaction_forms