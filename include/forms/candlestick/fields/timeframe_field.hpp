#pragma once
#include "dto/candlestick_query.hpp"
#include "forms/shared/fields/timeframe_field.hpp"

namespace candlestick
{
class TimeframeField : public forms::shared::TimeframeField<dto::CandlestickQuery>
{
  public:
    TimeframeField();
};
} // namespace candlestick