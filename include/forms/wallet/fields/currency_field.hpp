#pragma once
#include <set>
#include <string>

#include "core/ui/form/fields/text_field.hpp"

namespace wallet_forms
{

  class CurrencyField : public form::TextField
  {
  public:
    explicit CurrencyField(std::set<std::string, std::less<>> allowed_currencies);
  };

}
