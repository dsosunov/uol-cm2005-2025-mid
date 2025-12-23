#pragma once
#include <any>
#include <optional>
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace form
{

    class Field
    {
    public:
        virtual ~Field() = default;

        virtual const std::string &GetName() const = 0;
        virtual const std::string &GetPrompt() const = 0;
        virtual std::optional<std::string> GetValidationHint() const = 0;

        virtual ValidationResult Validate(const std::string &value, const FormContext &context) const = 0;
        virtual void BindValue(std::any &target, const std::string &value,
                               const FormContext &context) const = 0;
    };

}
