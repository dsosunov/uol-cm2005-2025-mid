#include "core/ui/form/form_builder.hpp"

namespace form {

FormBuilder::FormBuilder(std::shared_ptr<FormInputProvider> input_provider,
                         std::shared_ptr<Output> output)
    : input_provider_(std::move(input_provider)),
      output_(std::move(output)) {}

FormBuilder& FormBuilder::AddField(std::shared_ptr<Field> field) {
    fields_.push_back(std::move(field));
    return *this;
}

std::unique_ptr<Form> FormBuilder::Build() {
    return std::make_unique<Form>(std::move(fields_),
                                  input_provider_,
                                  output_);
}

} // namespace form
