#pragma once
#include <any>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace form {

enum class FormReadResult { kSuccess, kCancelled };

class Form {
 public:
  Form(std::vector<std::shared_ptr<Field>> fields,
       std::shared_ptr<FormInputProvider> input_provider, std::shared_ptr<Output> output);

  template <typename T>
  FormReadResult Read(T& target);

 private:
  std::vector<std::shared_ptr<Field>> fields_;
  std::shared_ptr<FormInputProvider> input_provider_;
  std::shared_ptr<Output> output_;
  FormContext context_;

  bool IsCancelKeyword(const std::string& value) const;
};

template <typename T>
FormReadResult Form::Read(T& target) {
  context_.Clear();
  std::any target_any = std::ref(target);

  for (const auto& field : fields_) {
    auto value_opt = field->ReadInput(*input_provider_, context_);

    if (!value_opt) {
      return FormReadResult::kCancelled;
    }

    const std::string& value = *value_opt;

    if (IsCancelKeyword(value)) {
      return FormReadResult::kCancelled;
    }

    auto validation = field->Validate(value, context_);
    if (!validation.is_valid) {
      output_->WriteLine("Error: " + validation.error_message);
      return FormReadResult::kCancelled;
    }

    field->BindValue(target_any, value, context_);
    context_.SetValue(field->GetName(), value);
  }

  return FormReadResult::kSuccess;
}

}  // namespace form
