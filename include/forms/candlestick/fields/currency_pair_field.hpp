#pragma once
#include <set>
#include <string>

#include "core/ui/form/fields/text_field.hpp"
#include "dto/candlestick_query.hpp"

namespace candlestick {

class CurrencyPairField : public form::TextField {
 public:
  explicit CurrencyPairField(std::set<std::string> allowed_currencies);
};

}  // namespace candlestick
