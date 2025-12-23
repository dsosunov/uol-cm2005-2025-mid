#pragma once
#include <optional>
#include <set>
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"

namespace forms::shared
{

  class CurrencyPairValidator : public form::Validator
  {
  public:
    explicit CurrencyPairValidator(std::set<std::string> allowed_currencies);

    form::ValidationResult Validate(const std::string &value,
                                    const form::FormContext &context) const override;

    std::optional<std::string> GetHint() const override;

  private:
    std::set<std::string> allowed_currencies_;
  };

} // namespace forms::shared
