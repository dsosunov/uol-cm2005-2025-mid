#pragma once
#include <optional>
#include <string>
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"
namespace user_forms
{
    class PasswordValidator : public form::Validator
    {
    public:
        form::ValidationResult Validate(const std::string &value,
                                        const form::FormContext &context) const override;
        std::optional<std::string> GetHint() const override;
    };
}