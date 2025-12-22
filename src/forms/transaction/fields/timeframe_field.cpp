#include "forms/transaction/fields/timeframe_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"

namespace transaction_forms
{

TimeframeField::TimeframeField()
    : form::SelectionField(
          "timeframe",
          "Select timeframe",
          std::vector<std::string>{"daily", "monthly", "yearly"},
          form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
              &dto::ActivitySummary::timeframe)) {}

}  // namespace transaction_forms
