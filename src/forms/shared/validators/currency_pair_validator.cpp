#include "forms/shared/validators/currency_pair_validator.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <string>

namespace forms::shared
{

namespace
{
std::string FormatAllowedCurrencies(const std::set<std::string, std::less<>>& allowed)
{
    if (allowed.empty())
    {
        return "Allowed currencies: (none)";
    }

    constexpr size_t kMaxToShow = 20;

    std::string result = "Allowed currencies: ";
    size_t shown = 0;
    for (const auto& currency : allowed)
    {
        if (shown > 0)
        {
            result += ", ";
        }

        if (shown >= kMaxToShow)
        {
            result += std::format("... (+{} more)", allowed.size() - shown);
            break;
        }

        result += currency;
        ++shown;
    }

    return result;
}
} // namespace

CurrencyPairValidator::CurrencyPairValidator(std::set<std::string, std::less<>> allowed_currencies)
    : allowed_currencies_(std::move(allowed_currencies))
{
}

form::ValidationResult CurrencyPairValidator::Validate(const std::string& value,
                                                       const form::FormContext& context) const
{
    size_t pos = value.find('/');
    if (pos == std::string::npos)
    {
        return form::ValidationResult::Invalid(std::format(
            "Format must be CUR1/CUR2. {}", FormatAllowedCurrencies(allowed_currencies_)));
    }

    std::string first = value.substr(0, pos);
    std::string second = value.substr(pos + 1);

    std::transform(first.begin(), first.end(), first.begin(), ::toupper);
    std::transform(second.begin(), second.end(), second.begin(), ::toupper);

    if (first.empty() || second.empty())
    {
        return form::ValidationResult::Invalid(std::format(
            "Format must be CUR1/CUR2. {}", FormatAllowedCurrencies(allowed_currencies_)));
    }

    if (first == second)
    {
        return form::ValidationResult::Invalid(
            std::format("Currency pair must contain two different currencies. {}",
                        FormatAllowedCurrencies(allowed_currencies_)));
    }

    if (!allowed_currencies_.contains(first))
    {
        return form::ValidationResult::Invalid(std::format(
            "Unknown currency: {}. {}", first, FormatAllowedCurrencies(allowed_currencies_)));
    }

    if (!allowed_currencies_.contains(second))
    {
        return form::ValidationResult::Invalid(std::format(
            "Unknown currency: {}. {}", second, FormatAllowedCurrencies(allowed_currencies_)));
    }

    return form::ValidationResult::Valid();
}

std::optional<std::string> CurrencyPairValidator::GetHint() const
{
    return "Enter currency pair in format CUR1/CUR2 (e.g., ETH/BTC)";
}

} // namespace forms::shared