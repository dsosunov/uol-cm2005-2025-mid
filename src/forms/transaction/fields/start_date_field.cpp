#include "forms/transaction/fields/start_date_field.hpp"

#include "dto/activity_summary.hpp"
#include "forms/shared/fields/date_fields.hpp"

namespace transaction_forms
{

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : forms::shared::StartDateField<dto::ActivitySummary>(std::move(data_source),
                                                          &dto::ActivitySummary::start_date)
{
}

} // namespace transaction_forms