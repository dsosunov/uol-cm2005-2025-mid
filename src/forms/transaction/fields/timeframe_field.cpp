#include "forms/transaction/fields/timeframe_field.hpp"

#include "dto/activity_summary.hpp"
#include "forms/shared/fields/timeframe_field.hpp"

namespace transaction_forms
{
TimeframeField::TimeframeField()
    : forms::shared::TimeframeField<dto::ActivitySummary>(&dto::ActivitySummary::timeframe)
{
}
} // namespace transaction_forms