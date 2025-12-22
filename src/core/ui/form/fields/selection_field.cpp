#include "core/ui/form/fields/selection_field.hpp"

namespace form
{

SelectionField::SelectionField(std::string name, std::string prompt,
                               std::vector<std::string> options,
                               ValueBinder binder)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      static_options_(std::move(options)),
      data_source_(nullptr),
      binder_(std::move(binder)),
      is_optional_(false) {}

SelectionField::SelectionField(std::string name, std::string prompt,
                               std::shared_ptr<DataSource> data_source,
                               ValueBinder binder)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      data_source_(std::move(data_source)),
      binder_(std::move(binder)),
      is_optional_(false) {}

const std::string& SelectionField::GetName() const { return name_; }

const std::string& SelectionField::GetPrompt() const { return prompt_; }

bool SelectionField::IsOptional() const { return is_optional_; }

ValidationResult SelectionField::Validate(const std::string& value,
                                         const FormContext& context) const {
  return ValidationResult::Valid();
}

std::vector<std::string> SelectionField::GetOptions(
    const FormContext& context) const {
  if (data_source_) {
    return data_source_->GetOptions(context);
  }
  return static_options_;
}

void SelectionField::BindValue(std::any& target, const std::string& value,
                               const FormContext& context) const {
  binder_(target, value, context);
}

bool SelectionField::HasOptions() const { return true; }

void SelectionField::SetOptional(bool optional) { is_optional_ = optional; }

}  // namespace form
