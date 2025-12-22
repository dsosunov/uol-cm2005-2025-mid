#include "core/ui/form/fields/text_field.hpp"

namespace form
{

TextField::TextField(std::string name, std::string prompt, ValueBinder binder)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      binder_(std::move(binder)),
      is_optional_(false) {}

const std::string& TextField::GetName() const { return name_; }

const std::string& TextField::GetPrompt() const { return prompt_; }

bool TextField::IsOptional() const { return is_optional_; }

ValidationResult TextField::Validate(const std::string& value,
                                     const FormContext& context) const {
  if (validator_) {
    return validator_(value, context);
  }
  return ValidationResult::Valid();
}

std::vector<std::string> TextField::GetOptions(const FormContext& context) const {
  return {};
}

void TextField::BindValue(std::any& target, const std::string& value,
                         const FormContext& context) const {
  binder_(target, value, context);
}

bool TextField::HasOptions() const { return false; }

void TextField::SetOptional(bool optional) { is_optional_ = optional; }

void TextField::SetValidator(FieldValidator validator) {
  validator_ = std::move(validator);
}

}  // namespace form
