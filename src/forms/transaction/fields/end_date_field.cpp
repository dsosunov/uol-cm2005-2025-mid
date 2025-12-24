#include "forms/transaction/fields/end_date_field.hpp"

#include "dto/activity_summary.hpp"
#include "forms/shared/fields/date_fields.hpp"

namespace transaction_forms
{

EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
    : forms::shared::EndDateField<dto::ActivitySummary>(std::move(data_source),
                                                        &dto::ActivitySummary::end_date)
{
}

} // namespace transaction_forms