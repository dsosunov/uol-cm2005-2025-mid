#include "forms/candlestick/candlestick_form.hpp"

#include "core/ui/form/contextual_data_source.hpp"
#include "core/ui/form/form_builder_utils.hpp"
#include "forms/candlestick/fields/asks_bids_field.hpp"
#include "forms/candlestick/fields/currency_pair_field.hpp"
#include "forms/candlestick/fields/end_date_field.hpp"
#include "forms/candlestick/fields/start_date_field.hpp"
#include "forms/candlestick/fields/timeframe_field.hpp"

namespace candlestick
{
CandlestickForm::CandlestickForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                 std::shared_ptr<Output> output,
                                 std::shared_ptr<CandlestickFormDataProvider> data_provider)
    : form::Form(SetupFormLayout(std::move(data_provider)), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> CandlestickForm::SetupFormLayout(
    std::shared_ptr<CandlestickFormDataProvider> data_provider)
{
    auto start_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext& form_context) {
            auto timeframe = form_context.GetValue("timeframe");
            return data_provider->GetStartDates(timeframe.value_or(""));
        });

    auto end_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext& form_context) {
            auto timeframe = form_context.GetValue("timeframe");
            auto start_date = form_context.GetValue("start_date");
            return data_provider->GetEndDates(timeframe.value_or(""), start_date.value_or(""));
        });

    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<CurrencyPairField>(fields, data_provider->GetAvailableProducts());
    form::AddField<AsksBidsField>(fields);
    form::AddField<TimeframeField>(fields);
    form::AddField<StartDateField>(fields, start_date_source);
    form::AddField<EndDateField>(fields, end_date_source);

    return fields;
}
} // namespace candlestick