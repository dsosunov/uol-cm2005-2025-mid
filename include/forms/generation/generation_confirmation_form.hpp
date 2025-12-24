#pragma once
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"
#include "dto/generation_request.hpp"

#include <memory>
#include <vector>

namespace generation_forms
{
class GenerationConfirmationForm : public form::Form
{
  public:
    GenerationConfirmationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                               std::shared_ptr<Output> output)
        : Form(SetupFormLayout(), input_provider, output)
    {
    }

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout()
    {
        std::vector<std::shared_ptr<form::Field>> fields;
        // Simple confirmation field - just press enter to confirm
        fields.push_back(std::make_shared<form::TextField>(
            "confirmation", "Press Enter to confirm or type 'cancel' to abort",
            [](std::any& target, const std::any& value, const form::FormContext&) {
                auto& data =
                    std::any_cast<std::reference_wrapper<dto::GenerationRequest>>(target).get();
                const auto& str = std::any_cast<std::string>(value);
                data.confirmation = str.empty() ? "yes" : str;
            },
            nullptr));
        return fields;
    }
};
} // namespace generation_forms
