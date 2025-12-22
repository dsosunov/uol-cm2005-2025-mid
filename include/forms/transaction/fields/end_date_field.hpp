#pragma once
#include "core/ui/form/fields/text_field.hpp"
#include <memory>
#include "core/ui/form/data_source.hpp"

namespace transaction_forms
{

class EndDateField : public form::TextField
{
public:
  explicit EndDateField(std::shared_ptr<form::DataSource> data_source);
};

}  // namespace transaction_forms
