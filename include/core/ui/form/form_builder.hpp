#pragma once
#include <memory>
#include <vector>
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/form/fields/field.hpp"
#include "core/ui/io/output.hpp"

namespace form {

/// Builder for creating Form instances with a fluent API.
/// Simplifies form construction by chaining field additions.
class FormBuilder {
public:
    FormBuilder(std::shared_ptr<FormInputProvider> input_provider,
                std::shared_ptr<Output> output);

    /// Add a field to the form
    FormBuilder& AddField(std::shared_ptr<Field> field);

    /// Build the final Form instance
    std::unique_ptr<Form> Build();

private:
    std::vector<std::shared_ptr<Field>> fields_;
    std::shared_ptr<FormInputProvider> input_provider_;
    std::shared_ptr<Output> output_;
};

} // namespace form
