#include "core/ui/form/fields/text_field.hpp"

namespace form
{

  TextField::TextField(std::string name, std::string prompt, ValueBinder binder,
                       std::shared_ptr<Validator> validator)
      : name_(std::move(name)),
        prompt_(std::move(prompt)),
        binder_(std::move(binder)),
        validator_(std::move(validator)) {}

  const std::string &TextField::GetName() const { return name_; }

  const std::string &TextField::GetPrompt() const { return prompt_; }

  std::optional<std::string> TextField::GetValidationHint() const
  {
    if (validator_)
    {
      return validator_->GetHint();
    }
    return std::nullopt;
  }

  ValidationResult TextField::Validate(const std::string &value, const FormContext &context) const
  {
    if (validator_)
    {
      return validator_->Validate(value, context);
    }
    return ValidationResult::Valid();
  }

  void TextField::BindValue(std::any &target, const std::string &value,
                            const FormContext &context) const
  {
    binder_(target, value, context);
  }

}
