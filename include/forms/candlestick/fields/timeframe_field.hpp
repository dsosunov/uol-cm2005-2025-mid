#pragma once
#include "core/ui/form/fields/selection_field.hpp"
#include "dto/candlestick_query.hpp"

namespace candlestick
{

class TimeframeField : public form::SelectionField
{
public:
  TimeframeField();
};

}  // namespace candlestick
