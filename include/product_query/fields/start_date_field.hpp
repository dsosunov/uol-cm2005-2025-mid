#pragma once
#include <memory>
#include "core/ui/form/fields/selection_field.hpp"
#include "core/ui/form/data_source.hpp"
#include "dto/product_query.hpp"

namespace product_query
{

class StartDateField : public form::SelectionField
{
public:
  explicit StartDateField(std::shared_ptr<form::DataSource> data_source);
};

}  // namespace product_query
