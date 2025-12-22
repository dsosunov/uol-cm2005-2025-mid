#pragma once
#include <any>
#include <string>
#include <functional>
#include "core/ui/form/form_context.hpp"
#include "dto/product_query.hpp"

namespace product_query
{

class CurrencyPairBinder {
 public:
  void operator()(std::any& target, const std::string& value,
                 const form::FormContext& context) const;
};

}  // namespace product_query
