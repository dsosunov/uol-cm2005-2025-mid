#pragma once
#include <optional>
#include <string>

namespace form
{
    class Field;
    class FormContext;
    class FormInputProvider
    {
    public:
        virtual ~FormInputProvider() = default;
        virtual std::optional<std::string> ReadField(const Field &field, const FormContext &context) = 0;
    };

}
