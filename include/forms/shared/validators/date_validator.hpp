#pragma once
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace forms::shared {

class DateValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

}  // namespace forms::shared
