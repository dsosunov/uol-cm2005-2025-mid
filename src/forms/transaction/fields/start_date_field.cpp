#include "forms/transaction/fields/start_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"

namespace transaction_forms {

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : form::SelectionField("start_date", "Select start date", std::move(data_source),
                           form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
                               &dto::ActivitySummary::start_date)) {}

}  // namespace transaction_forms
