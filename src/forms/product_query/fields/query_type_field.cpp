#include "forms/product_query/fields/query_type_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace product_query
{

QueryTypeField::QueryTypeField()
    : form::SelectionField(
          "query_type",
          "Select query type",
          std::vector<std::string>{"asks", "bids"},
          form::SimpleFieldBinder<dto::ProductQuery, std::string>(
              &dto::ProductQuery::query_type)) {}

}  // namespace product_query
