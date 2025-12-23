#include "forms/wallet/validators/amount_validator.hpp"

#include <cctype>
#include <stdexcept>

namespace wallet_forms
{

  form::ValidationResult AmountValidator::Validate(const std::string &value,
                                                   const form::FormContext &context) const
  {
    if (value.empty())
    {
      return form::ValidationResult::Invalid("Amount cannot be empty");
    }

    bool has_decimal = false;
    bool has_digit = false;
    size_t decimal_places = 0;

    for (size_t i = 0; i < value.length(); ++i)
    {
      char c = value[i];

      if (std::isdigit(c))
      {
        has_digit = true;
        if (has_decimal)
        {
          decimal_places++;
        }
      }
      else if (c == '.')
      {
        if (has_decimal)
        {
          return form::ValidationResult::Invalid("Amount can only have one decimal point");
        }
        has_decimal = true;
      }
      else
      {
        return form::ValidationResult::Invalid("Amount must be a valid number");
      }
    }

    if (!has_digit)
    {
      return form::ValidationResult::Invalid("Amount must contain at least one digit");
    }

    if (decimal_places > 2)
    {
      return form::ValidationResult::Invalid("Amount can have at most 2 decimal places");
    }

    try
    {
      double amount = std::stod(value);
      if (amount <= 0)
      {
        return form::ValidationResult::Invalid("Amount must be positive");
      }
      if (amount > 1000000000)
      {
        return form::ValidationResult::Invalid("Amount is too large");
      }
    }
    catch (const std::exception &)
    {
      return form::ValidationResult::Invalid("Invalid amount format");
    }

    return form::ValidationResult::Valid();
  }

  std::optional<std::string> AmountValidator::GetHint() const
  {
    return "Enter a positive number with up to 8 decimal places";
  }

} // namespace wallet_forms
