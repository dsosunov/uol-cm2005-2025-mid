#include "forms/candlestick/fields/timeframe_field.hpp"
#include "dto/candlestick_query.hpp"
#include "forms/shared/fields/timeframe_field.hpp"
namespace candlestick
{
    TimeframeField::TimeframeField()
        : forms::shared::TimeframeField<dto::CandlestickQuery>(
              &dto::CandlestickQuery::timeframe) {}
}