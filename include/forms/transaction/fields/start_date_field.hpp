#pragma once
#include "core/ui/form/fields/text_field.hpp"
#include <memory>
#include "core/ui/form/data_source.hpp"

namespace transaction_forms
{

class StartDateField : public form::TextField
{
public:
  explicit StartDateField(std::shared_ptr<form::DataSource> data_source);
};

}  // namespace transaction_forms
