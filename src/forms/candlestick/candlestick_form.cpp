#include "forms/candlestick/candlestick_form.hpp"

#include "core/ui/form/contextual_data_source.hpp"
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
      : form::Form(SetupFormLayout(std::move(data_provider)),
                   input_provider, output) {}

  std::vector<std::shared_ptr<form::Field>> CandlestickForm::SetupFormLayout(
      std::shared_ptr<CandlestickFormDataProvider> data_provider)
  {
    // Start date: ContextualDataSource queries context, calls provider method
    auto start_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext &form_context)
        {
          auto timeframe = form_context.GetValue("timeframe");
          return data_provider->GetStartDates(timeframe ? *timeframe : "");
        });

    // End date: ContextualDataSource queries context (timeframe + start_date), calls provider method
    auto end_date_source = std::make_shared<form::ContextualDataSource>(
        [data_provider](const form::FormContext &form_context)
        {
          auto timeframe = form_context.GetValue("timeframe");
          auto start_date = form_context.GetValue("start_date");
          return data_provider->GetEndDates(
              timeframe ? *timeframe : "",
              start_date ? *start_date : "");
        });

    std::vector<std::shared_ptr<form::Field>> fields;
    fields.push_back(std::make_shared<CurrencyPairField>(data_provider->GetAvailableCurrencies()));
    fields.push_back(std::make_shared<AsksBidsField>());
    fields.push_back(std::make_shared<TimeframeField>());
    fields.push_back(std::make_shared<StartDateField>(start_date_source));
    fields.push_back(std::make_shared<EndDateField>(end_date_source));

    return fields;
  }

} // namespace candlestick
