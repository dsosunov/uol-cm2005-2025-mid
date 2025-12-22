#pragma once
#include "core/ui/form/fields/selection_field.hpp"
#include "dto/product_query.hpp"

namespace product_query
{

class QueryTypeField : public form::SelectionField
{
public:
  QueryTypeField();
};

}  // namespace product_query
