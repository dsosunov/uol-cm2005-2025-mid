#pragma once
#include <optional>
#include <string>
#include <string_view>

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
        form::ValidationResult ValidateNumericString(std::string_view str, std::string_view field_name) const;
        form::ValidationResult ValidateDay(std::string_view day_str) const;
        form::ValidationResult ValidateMonth(std::string_view month_str) const;
        form::ValidationResult ValidateYear(std::string_view year_str) const;
        form::ValidationResult ValidateDMYFormat(std::string_view value) const;
        form::ValidationResult ValidateYMDFormat(std::string_view value) const;
    };

} // namespace forms::shared
