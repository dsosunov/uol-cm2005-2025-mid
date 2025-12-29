#include "forms/shared/validators/currency_validator.hpp"

#include "core/utils/string_utils.hpp"

#include <algorithm>
#include <cctype>
#include <format>

namespace forms::shared
{

CurrencyValidator::CurrencyValidator(std::set<std::string, std::less<>> allowed_currencies)
    : allowed_currencies_(std::move(allowed_currencies))
{
}

form::ValidationResult CurrencyValidator::Validate(const std::string& value,
                                                   const form::FormContext& context) const
{
    if (allowed_currencies_.empty())
    {
        return form::ValidationResult::Valid();
    }

    std::string upper_value = value;
    std::transform(upper_value.begin(), upper_value.end(), upper_value.begin(), ::toupper);

    if (!allowed_currencies_.contains(upper_value))
    {
        return form::ValidationResult::Invalid(std::format("Unknown currency: {}", value));
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> CurrencyValidator::GetHint() const
{
    if (allowed_currencies_.empty())
    {
        return std::nullopt;
    }

    return std::format("Enter a currency code ({})",
                       utils::StringUtils::Join(allowed_currencies_, ", "));
}

} // namespace forms::shared
