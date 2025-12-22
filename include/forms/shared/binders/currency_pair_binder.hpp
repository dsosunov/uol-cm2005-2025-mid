#pragma once
#include <any>
#include <string>
#include <functional>
#include "core/ui/form/form_context.hpp"

namespace forms::shared
{

/// Generic binder for currency pairs that have currency_base and currency_quote fields
/// Works with any DTO that has these two string members
template<typename DTO>
class CurrencyPairBinder {
 public:
  void operator()(std::any& target, const std::string& value,
                 const form::FormContext& context) const {
    auto& dto = std::any_cast<std::reference_wrapper<DTO>>(target).get();

    size_t pos = value.find('/');
    if (pos != std::string::npos) {
      dto.currency_base = value.substr(0, pos);
      dto.currency_quote = value.substr(pos + 1);
    }
  }
};

}  // namespace forms::shared
