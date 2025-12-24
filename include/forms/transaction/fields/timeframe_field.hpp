#pragma once
#include "dto/activity_summary.hpp"
#include "forms/shared/fields/timeframe_field.hpp"

namespace transaction_forms
{
class TimeframeField : public forms::shared::TimeframeField<dto::ActivitySummary>
{
  public:
    TimeframeField();
};
} // namespace transaction_forms