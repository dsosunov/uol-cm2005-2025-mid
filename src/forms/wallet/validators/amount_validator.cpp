#include "forms/wallet/validators/amount_validator.hpp"

#include "app_constants.hpp"

#include <cctype>
#include <format>
#include <stdexcept>

namespace wallet_forms
{

form::ValidationResult AmountValidator::Validate(const std::string& value,
                                                 const form::FormContext& context) const
{
    if (value.empty())
    {
        return form::ValidationResult::Invalid("Amount cannot be empty");
    }

    bool has_decimal = false;
    bool has_digit = false;
    size_t decimal_places = 0;

    for (char c : value)
    {
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

    if (decimal_places > app::kWalletAmountMaxDecimalPlaces)
    {
        return form::ValidationResult::Invalid(std::format(
            "Amount can have at most {} decimal places", app::kWalletAmountMaxDecimalPlaces));
    }

    try
    {
        double amount = std::stod(value);

        if (amount <= app::kWalletAmountMinExclusive)
        {
            return form::ValidationResult::Invalid("Amount must be positive");
        }

        if (amount > app::kWalletAmountMax)
        {
            return form::ValidationResult::Invalid("Amount is too large");
        }
    }
    catch (const std::exception&)
    {
        return form::ValidationResult::Invalid("Invalid amount format");
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> AmountValidator::GetHint() const
{
    return std::format("Enter a positive number with up to {} decimal places",
                       app::kWalletAmountHintMaxDecimalPlaces);
}

} // namespace wallet_forms