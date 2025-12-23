#pragma once
#include "core/ui/form/fields/selection_field.hpp"
#include "dto/candlestick_query.hpp"

namespace candlestick {

class AsksBidsField : public form::SelectionField {
 public:
  AsksBidsField();
};

}
