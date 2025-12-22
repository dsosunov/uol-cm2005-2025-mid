#include "product_query/binders/field_binders.hpp"
#include <functional>

namespace product_query
{

void CurrencyPairBinder::operator()(std::any& target,
                                   const std::string& value,
                                   const form::FormContext& context) const {
  auto& query = std::any_cast<std::reference_wrapper<dto::ProductQuery>>(
                    target)
                    .get();

  size_t pos = value.find('/');
  if (pos != std::string::npos) {
    query.currency_base = value.substr(0, pos);
    query.currency_quote = value.substr(pos + 1);
  }
}

}  // namespace product_query
