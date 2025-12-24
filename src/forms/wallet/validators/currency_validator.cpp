#include "forms/wallet/validators/currency_validator.hpp"

#include <format>

namespace wallet_forms
{

CurrencyValidator::CurrencyValidator(std::set<std::string, std::less<>> allowed_currencies)
    : allowed_currencies_(std::move(allowed_currencies))
{
}

form::ValidationResult CurrencyValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (!allowed_currencies_.contains(value))
    {
        return form::ValidationResult::Invalid(std::format("Unknown currency: {}", value));
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> CurrencyValidator::GetHint() const
{
    return "Enter one of the supported currency codes";
}

} // namespace wallet_forms