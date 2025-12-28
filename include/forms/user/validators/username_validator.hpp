#pragma once
#include "core/ui/form/validator.hpp"

#include <optional>
#include <string>

namespace user_forms
{
class UsernameValidator : public form::Validator
{
  public:
    std::optional<std::string> GetHint() const override;
    form::ValidationResult Validate(const std::string& value,
                                    const form::FormContext& context) const override;
};
} // namespace user_forms
