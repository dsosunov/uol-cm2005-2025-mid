#pragma once
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"

#include <optional>
#include <string>

namespace user_forms
{
class FullNameValidator : public form::Validator
{
  public:
    form::ValidationResult Validate(const std::string& value,
                                    const form::FormContext& context) const override;
    std::optional<std::string> GetHint() const override;
};
} // namespace user_forms