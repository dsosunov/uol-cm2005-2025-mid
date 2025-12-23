#pragma once

#include <memory>
#include <vector>

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace user_forms
{

  class ResetForm : public form::Form
  {
  public:
    ResetForm(std::shared_ptr<form::FormInputProvider> input_provider,
              std::shared_ptr<Output> output);

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout();
  };

}
