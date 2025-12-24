#pragma once
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

#include <memory>
#include <vector>

namespace user_forms
{
class LoginForm : public form::Form
{
  public:
    LoginForm(std::shared_ptr<form::FormInputProvider> input_provider,
              std::shared_ptr<Output> output);

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout();
};
} // namespace user_forms