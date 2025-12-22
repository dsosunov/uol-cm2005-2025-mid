#include "product_query/fields/end_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace product_query
{

EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
    : form::SelectionField(
          "end_date",
          "Select end date",
          std::move(data_source),
          form::SimpleFieldBinder<dto::ProductQuery, std::string>(
              &dto::ProductQuery::end_date)) {}

}  // namespace product_query
