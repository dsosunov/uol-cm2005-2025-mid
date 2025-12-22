#pragma once
#include <any>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_builder.hpp"

namespace form
{

enum class FormReadResult {
  kSuccess,
  kCancelled
};

class Form {
 public:
  Form(std::vector<std::shared_ptr<Field>> fields,
       std::shared_ptr<Input> input,
       std::shared_ptr<Output> output,
       MenuRenderer& renderer,
       MenuInput& menu_input);

  template<typename T>
  FormReadResult Read(T& target);

 private:
  std::vector<std::shared_ptr<Field>> fields_;
  std::shared_ptr<Input> input_;
  std::shared_ptr<Output> output_;
  MenuRenderer& renderer_;
  MenuInput& menu_input_;
  FormContext context_;

  bool IsCancelKeyword(const std::string& value) const;
};

template<typename T>
FormReadResult Form::Read(T& target) {
  context_.Clear();
  std::any target_any = std::ref(target);

  for (const auto& field : fields_) {
    std::string value;

    if (field->HasOptions()) {
      // Build menu for selection
      auto options = field->GetOptions(context_);

      MenuBuilder builder(field->GetPrompt());
      for (const auto& option : options) {
        builder.AddLeaf(option, nullptr);
      }
      auto menu = builder.Build();

      renderer_.RenderMenu(*menu);
      MenuNode* selected = menu_input_.ReadSelection(*menu);

      if (!selected || selected == menu.get()) {
        if (field->IsOptional()) {
          continue;
        }
        return FormReadResult::kCancelled;
      }

      value = selected->Title();
    } else {
      // Text input
      output_->Write(field->GetPrompt() + ": ");
      value = input_->ReadLine();

      if (IsCancelKeyword(value)) {
        return FormReadResult::kCancelled;
      }

      if (value.empty() && field->IsOptional()) {
        continue;
      }

      auto validation = field->Validate(value, context_);
      if (!validation.is_valid) {
        output_->WriteLine("Error: " + validation.error_message);
        return FormReadResult::kCancelled;
      }
    }

    field->BindValue(target_any, value, context_);
    context_.SetValue(field->GetName(), value);
  }

  return FormReadResult::kSuccess;
}

}  // namespace form
