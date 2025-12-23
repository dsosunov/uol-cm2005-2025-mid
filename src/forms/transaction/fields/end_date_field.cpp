#include "forms/transaction/fields/end_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/activity_summary.hpp"

namespace transaction_forms
{

    EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
        : form::SelectionField("end_date", "Select end date", std::move(data_source),
                               form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
                                   &dto::ActivitySummary::end_date)) {}

} // namespace transaction_forms
