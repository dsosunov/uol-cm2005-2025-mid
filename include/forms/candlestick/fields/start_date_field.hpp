#pragma once
#include <memory>
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/data_source.hpp"
#include "dto/candlestick_query.hpp"

namespace candlestick
{

class StartDateField : public form::TextField
{
public:
  explicit StartDateField(std::shared_ptr<form::DataSource> data_source);
};

}  // namespace candlestick
