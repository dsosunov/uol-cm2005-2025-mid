#pragma once
#include <set>
#include <string>
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/form_context.hpp"

namespace product_query
{

class CurrencyPairValidator {
 public:
  explicit CurrencyPairValidator(std::set<std::string> allowed_currencies);

  form::ValidationResult operator()(const std::string& value,
                            const form::FormContext& context) const;

 private:
  std::set<std::string> allowed_currencies_;
};

}  // namespace product_query
