#include "product_query/fields/start_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace product_query
{

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : form::SelectionField(
          "start_date",
          "Select start date",
          std::move(data_source),
          form::SimpleFieldBinder<dto::ProductQuery, std::string>(
              &dto::ProductQuery::start_date)) {}

}  // namespace product_query
