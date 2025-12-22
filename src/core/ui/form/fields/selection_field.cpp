#include "core/ui/form/fields/selection_field.hpp"

#include "core/ui/form/form_input_provider.hpp"

namespace form {

SelectionField::SelectionField(std::string name, std::string prompt,
                               std::vector<std::string> options, ValueBinder binder)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      static_options_(std::move(options)),
      data_source_(nullptr),
      binder_(std::move(binder)) {}

SelectionField::SelectionField(std::string name, std::string prompt,
                               std::shared_ptr<DataSource> data_source, ValueBinder binder)
    : name_(std::move(name)),
      prompt_(std::move(prompt)),
      data_source_(std::move(data_source)),
      binder_(std::move(binder)) {}

const std::string& SelectionField::GetName() const { return name_; }

const std::string& SelectionField::GetPrompt() const { return prompt_; }

std::optional<std::string> SelectionField::ReadInput(FormInputProvider& input_provider,
                                                     const FormContext& context) const {
  std::vector<std::string> options;
  if (data_source_) {
    options = data_source_->GetOptions(context);
  } else {
    options = static_options_;
  }
  return input_provider.ReadSelection(prompt_, options);
}

ValidationResult SelectionField::Validate(const std::string& value,
                                          const FormContext& context) const {
  return ValidationResult::Valid();
}

void SelectionField::BindValue(std::any& target, const std::string& value,
                               const FormContext& context) const {
  binder_(target, value, context);
}

}  // namespace form
