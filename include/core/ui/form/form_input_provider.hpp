#pragma once
#include <optional>
#include <string>

namespace form
{
    class Field;
    class FormContext;

    /// Pure adapter for reading field values.
    /// Provider introspects field type and adapts reading mechanism accordingly.
    /// Does NOT display prompts, hints, or check cancellation - Form owns that.
    class FormInputProvider
    {
    public:
        virtual ~FormInputProvider() = default;

        /// Reads field value based on field type.
        /// Returns nullopt if no input (error/unknown field type).
        /// Returns empty string for valid empty input.
        /// Context needed for evaluating dynamic data sources in SelectionField.
        virtual std::optional<std::string> ReadField(const Field &field, const FormContext &context) = 0;
    };

} // namespace form
