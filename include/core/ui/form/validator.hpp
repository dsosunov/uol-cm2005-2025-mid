#pragma once
#include <optional>
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace form
{
    class Validator
    {
    public:
        virtual ~Validator() = default;
        virtual ValidationResult Validate(const std::string &value,
                                          const FormContext &context) const = 0;
        virtual std::optional<std::string> GetHint() const { return std::nullopt; }
    };

}
