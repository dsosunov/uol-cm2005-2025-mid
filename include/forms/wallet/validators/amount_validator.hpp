#pragma once
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"

#include <optional>
#include <string>

namespace wallet_forms
{
class AmountValidator : public form::Validator
{
  public:
    form::ValidationResult Validate(const std::string& value,
                                    const form::FormContext& context) const override;
    std::optional<std::string> GetHint() const override;
};
} // namespace wallet_forms