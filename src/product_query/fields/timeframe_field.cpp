#include "product_query/fields/timeframe_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace product_query
{

TimeframeField::TimeframeField()
    : form::SelectionField(
          "timeframe",
          "Select timeframe",
          std::vector<std::string>{"daily", "monthly", "yearly"},
          form::SimpleFieldBinder<dto::ProductQuery, std::string>(
              &dto::ProductQuery::timeframe)) {}

}  // namespace product_query
