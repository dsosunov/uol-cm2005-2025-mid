#pragma once
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace user_forms
{
class RegistrationForm : public form::Form
{
  public:
    RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                     std::shared_ptr<Output> output);
    RegistrationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                     std::shared_ptr<Output> output, std::optional<std::string> default_username);

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
        const std::optional<std::string>& default_username);
};
} // namespace user_forms