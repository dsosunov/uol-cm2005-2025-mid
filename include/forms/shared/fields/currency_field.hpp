#pragma once
#include "core/ui/form/field_validator.hpp"
#include "core/ui/form/fields/text_field.hpp"

#include <memory>
#include <set>
#include <string>

namespace forms::shared
{
template <typename TDTO> class CurrencyField : public form::TextField
{
  public:
    using MemberPtr = std::string TDTO::*;
    explicit CurrencyField(std::set<std::string, std::less<>> allowed_currencies,
                           MemberPtr member_ptr);
};
} // namespace forms::shared
