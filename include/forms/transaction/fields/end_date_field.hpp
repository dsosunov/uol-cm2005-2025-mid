#pragma once
#include <memory>

#include "core/ui/form/data_source.hpp"
#include "core/ui/form/fields/selection_field.hpp"

namespace transaction_forms {

class EndDateField : public form::SelectionField {
 public:
  explicit EndDateField(std::shared_ptr<form::DataSource> data_source);
};

}  // namespace transaction_forms
