#include "actions/product_query_action.hpp"
#include "forms/product_query/product_query_form.hpp"
#include <set>

static const std::set<std::string> kAllowedCurrencies = {
    "USD", "CAD", "EUR", "GBP", "JPY", "AUD", "CHF", "CNY"};

void ProductQueryAction::Execute(ActionContext& context) {
  context.output->WriteLine("=== Product Query ===");
  context.output->WriteLine("Type 'cancel' to abort at any time");
  context.output->WriteLine("");

  dto::ProductQuery query;
  product_query::ProductQueryForm form(context.form_input_provider,
                                       context.output,
                                       kAllowedCurrencies);

  form::FormReadResult result = form.Read(query);

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
