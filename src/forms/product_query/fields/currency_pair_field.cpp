#include "forms/product_query/fields/currency_pair_field.hpp"
#include "forms/product_query/validators/currency_pair_validator.hpp"
#include "forms/product_query/binders/field_binders.hpp"

namespace product_query
{

CurrencyPairField::CurrencyPairField(std::set<std::string> allowed_currencies)
    : form::TextField("product_pair", "Enter product pair (e.g., USD/CAD)",
                      CurrencyPairBinder()) {
  SetValidator(CurrencyPairValidator(std::move(allowed_currencies)));
}

}  // namespace product_query
