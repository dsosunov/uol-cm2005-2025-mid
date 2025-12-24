#pragma once
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace wallet_forms
{
class WalletOperationForm : public form::Form
{
  public:
    WalletOperationForm(std::shared_ptr<form::FormInputProvider> input_provider,
                        std::shared_ptr<Output> output,
                        const std::set<std::string, std::less<>>& allowed_currencies);

  private:
    static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
        const std::set<std::string, std::less<>>& allowed_currencies);
};
} // namespace wallet_forms