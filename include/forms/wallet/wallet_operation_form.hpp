#ifndef FORMS_WALLET_WALLET_OPERATION_FORM_HPP_
#define FORMS_WALLET_WALLET_OPERATION_FORM_HPP_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace wallet_forms {

class WalletOperationForm : public form::Form {
 public:
  WalletOperationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                      std::shared_ptr<Output> output,
                      const std::set<std::string>& allowed_currencies);

 private:
  static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
      const std::set<std::string>& allowed_currencies);
};

}  // namespace wallet_forms

#endif  // FORMS_WALLET_WALLET_OPERATION_FORM_HPP_
