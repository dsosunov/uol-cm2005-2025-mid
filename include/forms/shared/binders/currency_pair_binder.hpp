#pragma once
#include "core/ui/form/form_context.hpp"

#include <algorithm>
#include <any>
#include <cctype>
#include <functional>
#include <string>

namespace forms::shared
{
template <typename DTO> class CurrencyPairBinder
{
  public:
    void operator()(std::any& target, const std::any& value, const form::FormContext&) const
    {
        auto& dto = std::any_cast<std::reference_wrapper<DTO>>(target).get();
        const auto& str = std::any_cast<std::string>(value);
        size_t pos = str.find('/');
        if (pos != std::string::npos)
        {
            std::string base(str.substr(0, pos));
            std::string quote(str.substr(pos + 1));
            std::transform(base.begin(), base.end(), base.begin(), ::toupper);
            std::transform(quote.begin(), quote.end(), quote.begin(), ::toupper);
            dto.currency_base = base;
            dto.currency_quote = quote;
        }
    }
};
} // namespace forms::shared