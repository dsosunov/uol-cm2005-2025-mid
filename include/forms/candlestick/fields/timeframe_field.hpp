#pragma once
#include "forms/shared/fields/timeframe_field.hpp"
#include "dto/candlestick_query.hpp"
namespace candlestick
{
  class TimeframeField : public forms::shared::TimeframeField<dto::CandlestickQuery>
  {
  public:
    TimeframeField();
  };
}