#pragma once

#include <memory>
#include <vector>

#include "core/ui/form/form.hpp"
#include "forms/transaction/activity_summary_form_data_provider.hpp"

namespace transaction_forms
{

  class ActivitySummaryForm : public form::Form
  {
  public:
    ActivitySummaryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                        std::shared_ptr<Output> output,
                        std::shared_ptr<ActivitySummaryFormDataProvider> data_provider);

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
        std::shared_ptr<ActivitySummaryFormDataProvider> data_provider);
  };

}
