#pragma once
#include <algorithm>
#include <any>
#include <cctype>
#include <functional>
#include <string>

#include "core/ui/form/form_context.hpp"

namespace forms::shared
{

  template <typename DTO>
  class CurrencyPairBinder
  {
  public:
    void operator()(std::any &target, const std::string &value,
                    const form::FormContext &) const
    {
      auto &dto = std::any_cast<std::reference_wrapper<DTO>>(target).get();

      size_t pos = value.find('/');
      if (pos != std::string::npos)
      {
        std::string base = value.substr(0, pos);
        std::string quote = value.substr(pos + 1);

        // Convert to uppercase
        std::transform(base.begin(), base.end(), base.begin(), ::toupper);
        std::transform(quote.begin(), quote.end(), quote.begin(), ::toupper);

        dto.currency_base = base;
        dto.currency_quote = quote;
      }
    }
  };

} // namespace forms::shared
