#pragma once
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

#include <any>
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <vector>

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
    template <typename T> FormReadResult Read(T& target);

  private:
    std::vector<std::shared_ptr<Field>> fields_;
    std::shared_ptr<FormInputProvider> input_provider_;
    std::shared_ptr<Output> output_;
    FormContext context_;
};
template <typename T> FormReadResult Form::Read(T& target)
{
    output_->WriteLine("");
    output_->WriteLine("(Type 'cancel' to abort)");
    output_->WriteLine("");
    context_.Clear();
    std::any target_any = std::ref(target);
    for (const auto& field : fields_)
    {
        if (auto hint = field->GetValidationHint(); hint)
        {
            output_->WriteLine(std::format("[{}]", *hint));
        }

        std::string prompt = field->GetPrompt();
        if (auto* text_field = dynamic_cast<const TextField*>(field.get()))
        {
            if (auto default_value = text_field->GetDefaultValue(); default_value)
            {
                prompt = std::format("{} [{}]", prompt, *default_value);
            }
        }

        output_->Write(std::format("{}: ", prompt));
        auto value_any = input_provider_->ReadField(*field, context_);
        if (!value_any)
        {
            output_->WriteLine("Error: Failed to read input");
            return FormReadResult::kCancelled;
        }

        // Apply default value for text fields when the user submits an empty line.
        std::any value_to_bind = *value_any;
        if (auto* text_field = dynamic_cast<const TextField*>(field.get()))
        {
            try
            {
                const auto& raw = std::any_cast<const std::string&>(*value_any);
                if (raw.empty())
                {
                    if (auto default_value = text_field->GetDefaultValue(); default_value)
                    {
                        value_to_bind = *default_value;
                    }
                }
            }
            catch (const std::bad_any_cast&)
            {
            }
        }

        try
        {
            const auto& input = std::any_cast<const std::string&>(value_to_bind);
            if (input == "cancel" || input == "Cancel" || input == "CANCEL")
            {
                return FormReadResult::kCancelled;
            }
            if (auto validation = field->Validate(input, context_); !validation.is_valid)
            {
                output_->WriteLine(std::format("Error: {}", validation.error_message));
                return FormReadResult::kCancelled;
            }
        }
        catch (const std::bad_any_cast&)
        {
        }

        field->BindValue(target_any, value_to_bind, context_);
        context_.SetValue(field->GetName(), value_to_bind);
    }
    return FormReadResult::kSuccess;
}
} // namespace form