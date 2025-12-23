#include "forms/wallet/validators/currency_validator.hpp"

namespace wallet_forms
{

  CurrencyValidator::CurrencyValidator(std::set<std::string> allowed_currencies)
      : allowed_currencies_(std::move(allowed_currencies)) {}

  form::ValidationResult CurrencyValidator::Validate(const std::string &value,
                                                     const form::FormContext &context) const
  {
    if (allowed_currencies_.find(value) == allowed_currencies_.end())
    {
      return form::ValidationResult::Invalid("Unknown currency: " + value);
    }
    return form::ValidationResult::Valid();
  }

  std::optional<std::string> CurrencyValidator::GetHint() const
  {
    return "Enter one of the supported currency codes";
  }

} // namespace wallet_forms
