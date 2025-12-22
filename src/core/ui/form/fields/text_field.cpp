#include "core/ui/form/fields/text_field.hpp"

#include "core/ui/form/form_input_provider.hpp"

namespace form {

TextField::TextField(std::string name, std::string prompt, ValueBinder binder,
                     FieldValidator validator)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      binder_(std::move(binder)),
      validator_(std::move(validator)) {}

const std::string& TextField::GetName() const { return name_; }

const std::string& TextField::GetPrompt() const { return prompt_; }

std::optional<std::string> TextField::ReadInput(FormInputProvider& input_provider,
                                                const FormContext& context) const {
  return input_provider.ReadText(prompt_);
}

ValidationResult TextField::Validate(const std::string& value, const FormContext& context) const {
  if (validator_) {
    return validator_(value, context);
  }
  return ValidationResult::Valid();
}

void TextField::BindValue(std::any& target, const std::string& value,
                          const FormContext& context) const {
  binder_(target, value, context);
}

}  // namespace form
