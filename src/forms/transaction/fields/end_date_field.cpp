#include "forms/transaction/fields/end_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"

namespace transaction_forms
{

EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
    : form::TextField("end_date", "Enter end date",
                      form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
                          &dto::ActivitySummary::end_date)) {}

}  // namespace transaction_forms
