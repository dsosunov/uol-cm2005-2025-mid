#include "product_query/product_query_form.hpp"
#include <set>
#include "product_query/fields/currency_pair_field.hpp"
#include "product_query/fields/query_type_field.hpp"
#include "product_query/fields/timeframe_field.hpp"
#include "product_query/fields/start_date_field.hpp"
#include "product_query/fields/end_date_field.hpp"

namespace product_query
{

static const std::set<std::string> kAllowedCurrencies = {
    "USD", "CAD", "EUR", "GBP", "JPY", "AUD", "CHF", "CNY"};

ProductQueryForm::ProductQueryForm(
    std::shared_ptr<Input> input,
    std::shared_ptr<Output> output,
    MenuRenderer& renderer,
    MenuInput& menu_input,
    std::shared_ptr<form::DataSource> start_date_source,
    std::shared_ptr<form::DataSource> end_date_source)
    : form::Form([&]() {
        std::vector<std::shared_ptr<form::Field>> fields;

        // Currency pair field
        fields.push_back(std::make_shared<CurrencyPairField>(kAllowedCurrencies));

        // Query type selection
        fields.push_back(std::make_shared<QueryTypeField>());

        // Timeframe selection
        fields.push_back(std::make_shared<TimeframeField>());

        // Start date selection (context-aware)
        auto start_date_field = std::make_shared<StartDateField>(start_date_source);
        start_date_field->SetOptional(true);
        fields.push_back(start_date_field);

        // End date selection (context-aware)
        auto end_date_field = std::make_shared<EndDateField>(end_date_source);
        end_date_field->SetOptional(true);
        fields.push_back(end_date_field);

        return fields;
      }(),
      input, output, renderer, menu_input) {}

}  // namespace product_query
