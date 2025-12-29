#pragma once
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

#include <any>
#include <cctype>
#include <format>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
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
    static void WriteValidationHint(Output& output, const Field& field);
    static std::string BuildPrompt(const Field& field);
    static std::any ApplyDefaultForEmptyTextField(const Field& field, const std::any& value);
    static bool IsCancelInput(std::string_view input);

    std::vector<std::shared_ptr<Field>> fields_;
    std::shared_ptr<FormInputProvider> input_provider_;
    std::shared_ptr<Output> output_;
    FormContext context_;
};

inline void Form::WriteValidationHint(Output& output, const Field& field)
{
    if (auto hint = field.GetValidationHint(); hint)
    {
        output.WriteLine(std::format("[{}]", *hint));
    }
}

inline std::string Form::BuildPrompt(const Field& field)
{
    std::string prompt = field.GetPrompt();
    if (const auto* text_field = dynamic_cast<const TextField*>(&field))
    {
        if (auto default_value = text_field->GetDefaultValue(); default_value)
        {
            prompt = std::format("{} [{}]", prompt, *default_value);
        }
    }
    return prompt;
}

inline std::any Form::ApplyDefaultForEmptyTextField(const Field& field, const std::any& value)
{
    const auto* text_field = dynamic_cast<const TextField*>(&field);
    if (!text_field)
    {
        return value;
    }

    if (const auto* raw = std::any_cast<const std::string>(&value); !raw || !raw->empty())
    {
        return value;
    }

    if (auto default_value = text_field->GetDefaultValue(); default_value)
    {
        return *default_value;
    }

    return value;
}

inline bool Form::IsCancelInput(std::string_view input)
{
    constexpr std::string_view kCancel = "cancel";
    if (input.size() != kCancel.size())
    {
        return false;
    }

    for (size_t i = 0; i < kCancel.size(); ++i)
    {
        const auto ch = static_cast<unsigned char>(input[i]);
        if (static_cast<char>(std::tolower(ch)) != kCancel[i])
        {
            return false;
        }
    }
    return true;
}
template <typename T> FormReadResult Form::Read(T& target)
{
    output_->WriteLine("");
    output_->WriteLine("(Type 'cancel' to abort)");
    output_->WriteLine("");
    context_.Clear();
    auto target_any = std::any{std::ref(target)};
    for (const auto& field : fields_)
    {
        WriteValidationHint(*output_, *field);

        const auto prompt = BuildPrompt(*field);
        output_->Write(std::format("{}: ", prompt));

        const auto value_any = input_provider_->ReadField(*field, context_);
        if (!value_any)
        {
            output_->WriteLine("Error: Failed to read input");
            return FormReadResult::kCancelled;
        }

        // Apply default value for text fields when the user submits an empty line.
        const auto value_to_bind = ApplyDefaultForEmptyTextField(*field, *value_any);

        if (const auto* input = std::any_cast<const std::string>(&value_to_bind))
        {
            if (IsCancelInput(*input))
            {
                return FormReadResult::kCancelled;
            }
            const auto validation = field->Validate(*input, context_);
            if (!validation.is_valid)
            {
                output_->WriteLine(std::format("Error: {}", validation.error_message));
                return FormReadResult::kCancelled;
            }
        }

        field->BindValue(target_any, value_to_bind, context_);
        context_.SetValue(field->GetName(), value_to_bind);
    }
    return FormReadResult::kSuccess;
}
} // namespace form