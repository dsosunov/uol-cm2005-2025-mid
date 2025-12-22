#ifndef FORMS_TRANSACTION_ACTIVITY_SUMMARY_FORM_HPP_
#define FORMS_TRANSACTION_ACTIVITY_SUMMARY_FORM_HPP_

#include <memory>
#include <vector>

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace transaction_forms {

class ActivitySummaryForm : public form::Form {
 public:
  ActivitySummaryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                      std::shared_ptr<Output> output);

 private:
  static std::vector<std::shared_ptr<form::Field>> SetupFormLayout();
};

}  // namespace transaction_forms

#endif  // FORMS_TRANSACTION_ACTIVITY_SUMMARY_FORM_HPP_
