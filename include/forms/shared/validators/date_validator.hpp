#pragma once
#include <optional>
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"

namespace forms::shared
{

    class DateValidator : public form::Validator
    {
    public:
        form::ValidationResult Validate(const std::string &value,
                                        const form::FormContext &context) const override;
        std::optional<std::string> GetHint() const override;

    private:
        form::ValidationResult ValidateNumericString(const std::string &str, const std::string &field_name) const;
        form::ValidationResult ValidateDay(const std::string &day_str) const;
        form::ValidationResult ValidateMonth(const std::string &month_str) const;
        form::ValidationResult ValidateYear(const std::string &year_str) const;
        form::ValidationResult ValidateDMYFormat(const std::string &value) const;
        form::ValidationResult ValidateYMDFormat(const std::string &value) const;
    };

} // namespace forms::shared
