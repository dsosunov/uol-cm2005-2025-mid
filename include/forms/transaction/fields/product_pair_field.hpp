#pragma once
#include <set>
#include <string>

#include "core/ui/form/fields/text_field.hpp"

namespace transaction_forms {

class ProductPairField : public form::TextField {
 public:
  explicit ProductPairField(std::set<std::string> allowed_currencies);
};

}  // namespace transaction_forms
