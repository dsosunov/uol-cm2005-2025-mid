#pragma once
#include "forms/shared/fields/timeframe_field.hpp"
#include "dto/activity_summary.hpp"
namespace transaction_forms
{
  class TimeframeField : public forms::shared::TimeframeField<dto::ActivitySummary>
  {
  public:
    TimeframeField();
  };
}