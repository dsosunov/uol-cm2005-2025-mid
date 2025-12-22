#pragma once
#include <set>
#include <string>
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/form_context.hpp"

namespace forms::shared
{

/// Shared validator for currency pair format (e.g., "USD/EUR")
class CurrencyPairValidator {
 public:
  explicit CurrencyPairValidator(std::set<std::string> allowed_currencies);

  form::ValidationResult operator()(const std::string& value,
                            const form::FormContext& context) const;

 private:
  std::set<std::string> allowed_currencies_;
};

}  // namespace forms::shared
