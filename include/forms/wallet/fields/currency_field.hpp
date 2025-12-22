#pragma once
#include "core/ui/form/fields/text_field.hpp"
#include <set>
#include <string>

namespace wallet_forms
{

class CurrencyField : public form::TextField
{
public:
  explicit CurrencyField(std::set<std::string> allowed_currencies);
};

}  // namespace wallet_forms
