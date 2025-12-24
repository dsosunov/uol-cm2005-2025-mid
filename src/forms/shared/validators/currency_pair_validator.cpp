#include "forms/shared/validators/currency_pair_validator.hpp"

#include <algorithm>
#include <cctype>
#include <format>

namespace forms::shared
{
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
        return form::ValidationResult::Invalid("Format must be CUR1/CUR2");
    }
    std::string first = value.substr(0, pos);
    std::string second = value.substr(pos + 1);
    std::transform(first.begin(), first.end(), first.begin(), ::toupper);
    std::transform(second.begin(), second.end(), second.begin(), ::toupper);
    if (!allowed_currencies_.contains(first))
    {
        return form::ValidationResult::Invalid(std::format("Unknown currency: {}", first));
    }
    if (!allowed_currencies_.contains(second))
    {
        return form::ValidationResult::Invalid(std::format("Unknown currency: {}", second));
    }
    return form::ValidationResult::Valid();
}
std::optional<std::string> CurrencyPairValidator::GetHint() const
{
    return "Enter currency pair in format CUR1/CUR2 (e.g., USD/CAD)";
}
} // namespace forms::shared