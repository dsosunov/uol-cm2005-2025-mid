#pragma once
#include <optional>
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace form
{

    /// Base interface for all validators.
    /// Validators define what input is valid and provide hints about requirements.
    class Validator
    {
    public:
        virtual ~Validator() = default;

        /// Validates the given value. Returns Valid() or Invalid(message).
        virtual ValidationResult Validate(const std::string &value,
                                          const FormContext &context) const = 0;

        /// Returns a hint describing what input is valid, or nullopt if no hint available.
        /// Example: "3-20 characters, letters, numbers, and underscores only"
        virtual std::optional<std::string> GetHint() const { return std::nullopt; }
    };

} // namespace form
