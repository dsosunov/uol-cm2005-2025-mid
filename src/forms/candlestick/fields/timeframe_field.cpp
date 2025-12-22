#include "forms/candlestick/fields/timeframe_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/constants.hpp"

namespace candlestick {

TimeframeField::TimeframeField()
    : form::SelectionField("timeframe", "Select timeframe", dto::timeframe::GetAll(),
                           form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
                               &dto::CandlestickQuery::timeframe)) {}

}  // namespace candlestick
