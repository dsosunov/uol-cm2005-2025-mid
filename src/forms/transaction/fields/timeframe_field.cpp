#include "forms/transaction/fields/timeframe_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/activity_summary.hpp"
#include "dto/constants.hpp"

namespace transaction_forms
{

    TimeframeField::TimeframeField()
        : form::SelectionField("timeframe", "Select timeframe", dto::timeframe::GetAll(),
                               form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
                                   &dto::ActivitySummary::timeframe)) {}

} // namespace transaction_forms
