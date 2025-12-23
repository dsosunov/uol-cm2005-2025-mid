#pragma once
#include <any>
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace form
{

  enum class FormReadResult
  {
    kSuccess,
    kCancelled
  };

  class Form
  {
  public:
    Form(std::vector<std::shared_ptr<Field>> fields,
         std::shared_ptr<FormInputProvider> input_provider, std::shared_ptr<Output> output);

    template <typename T>
    FormReadResult Read(T &target);

  private:
    std::vector<std::shared_ptr<Field>> fields_;
    std::shared_ptr<FormInputProvider> input_provider_;
    std::shared_ptr<Output> output_;
    FormContext context_;
  };

  template <typename T>
  FormReadResult Form::Read(T &target)
  {
    // Form owns all display and flow control
    output_->WriteLine("Type 'cancel' to abort at any time");
    output_->WriteLine("");

    context_.Clear();
    std::any target_any = std::ref(target);

    for (const auto &field : fields_)
    {
      // Form displays validation hint
      if (auto hint = field->GetValidationHint(); hint)
      {
        output_->WriteLine(std::format("[{}]", *hint));
      }

      // Form displays prompt
      output_->Write(std::format("{}: ", field->GetPrompt()));

      // Provider adapts based on field type, receives context for data sources
      auto valueOpt = input_provider_->ReadField(*field, context_);

      // Check if provider returned no input (error/unknown field type)
      if (!valueOpt)
      {
        output_->WriteLine("Error: Failed to read input");
        return FormReadResult::kCancelled;
      }

      std::string value = *valueOpt;

      // Form checks cancellation
      if (value == "cancel" || value == "Cancel" || value == "CANCEL")
      {
        return FormReadResult::kCancelled;
      }

      // Form validates
      if (auto validation = field->Validate(value, context_); !validation.is_valid)
      {
        output_->WriteLine(std::format("Error: {}", validation.error_message));
        return FormReadResult::kCancelled;
      }

      field->BindValue(target_any, value, context_);
      context_.SetValue(field->GetName(), value);
    }

    return FormReadResult::kSuccess;
  }

} // namespace form
