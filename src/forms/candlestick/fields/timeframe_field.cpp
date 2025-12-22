#include "forms/candlestick/fields/timeframe_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

TimeframeField::TimeframeField()
    : form::SelectionField(
          "timeframe",
          "Select timeframe",
          std::vector<std::string>{"daily", "monthly", "yearly"},
          form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
              &dto::CandlestickQuery::timeframe)) {}

}  // namespace candlestick
