#pragma once
#include <set>
#include <string>
#include "core/ui/form/fields/text_field.hpp"
#include "dto/product_query.hpp"

namespace product_query
{

class CurrencyPairField : public form::TextField
{
public:
  explicit CurrencyPairField(std::set<std::string> allowed_currencies);
};

}  // namespace product_query
