#include "forms/candlestick/candlestick_form.hpp"

#include "core/ui/form/contextual_data_source.hpp"
#include "dto/constants.hpp"
#include "forms/candlestick/fields/asks_bids_field.hpp"
#include "forms/candlestick/fields/currency_pair_field.hpp"
#include "forms/candlestick/fields/end_date_field.hpp"
#include "forms/candlestick/fields/start_date_field.hpp"
#include "forms/candlestick/fields/timeframe_field.hpp"

namespace candlestick
{

  CandlestickForm::CandlestickForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                   std::shared_ptr<Output> output,
                                   const std::set<std::string, std::less<>> &allowed_currencies)
      : form::Form(SetupFormLayout(allowed_currencies), input_provider, output) {}

  std::vector<std::shared_ptr<form::Field>> CandlestickForm::SetupFormLayout(
      const std::set<std::string, std::less<>> &allowed_currencies)
  {
    auto start_date_source = std::make_shared<form::ContextualDataSource>(
        [](const form::FormContext &form_context) -> std::vector<std::string>
        {
          auto timeframe = form_context.GetValue("timeframe");
          if (timeframe && *timeframe == dto::timeframe::MONTHLY)
          {
            return dto::sample_dates::GetMonthlySamples();
          }
          else if (timeframe && *timeframe == dto::timeframe::YEARLY)
          {
            return dto::sample_dates::GetYearlySamples();
          }
          return dto::sample_dates::GetDailySamples();
        });

    auto end_date_source = std::make_shared<form::ContextualDataSource>(
        [](const form::FormContext &form_context) -> std::vector<std::string>
        {
          auto timeframe = form_context.GetValue("timeframe");
          if (timeframe && *timeframe == dto::timeframe::MONTHLY)
          {
            return dto::sample_dates::GetMonthlySamples();
          }
          else if (timeframe && *timeframe == dto::timeframe::YEARLY)
          {
            return dto::sample_dates::GetYearlySamples();
          }
          return dto::sample_dates::GetDailySamples();
        });

    std::vector<std::shared_ptr<form::Field>> fields;
    fields.push_back(std::make_shared<CurrencyPairField>(allowed_currencies));
    fields.push_back(std::make_shared<AsksBidsField>());
    fields.push_back(std::make_shared<TimeframeField>());
    fields.push_back(std::make_shared<StartDateField>(start_date_source));
    fields.push_back(std::make_shared<EndDateField>(end_date_source));

    return fields;
  }

} // namespace candlestick
