#pragma once
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace wallet_forms {

class AmountValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

}  // namespace wallet_forms
