#include "forms/transaction/fields/start_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/activity_summary.hpp"

namespace transaction_forms
{

    StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
        : form::SelectionField("start_date", "Select start date", std::move(data_source),
                               form::DateFieldBinder<dto::ActivitySummary>(
                                   &dto::ActivitySummary::start_date)) {}

}
