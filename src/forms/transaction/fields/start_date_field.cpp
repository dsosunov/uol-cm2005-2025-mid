#include "forms/transaction/fields/start_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/transaction_query.hpp"

namespace transaction_forms
{

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : form::TextField("start_date", "Enter start date",
                      form::SimpleFieldBinder<dto::ActivitySummary, std::string>(
                          &dto::ActivitySummary::start_date)) {}

}  // namespace transaction_forms
