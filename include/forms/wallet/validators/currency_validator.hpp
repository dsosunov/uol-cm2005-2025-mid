#pragma once
#include <optional>
#include <set>
#include <string>
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/validator.hpp"
namespace wallet_forms
{
  class CurrencyValidator : public form::Validator
  {
  public:
    explicit CurrencyValidator(std::set<std::string, std::less<>> allowed_currencies);
    form::ValidationResult Validate(const std::string &value,
                                    const form::FormContext &context) const override;
    std::optional<std::string> GetHint() const override;
  private:
    std::set<std::string, std::less<>> allowed_currencies_;
  };
}