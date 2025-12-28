#pragma once
#include "core/ui/form/fields/text_field.hpp"

#include <optional>
#include <string>

namespace user_forms
{
class UsernameField : public form::TextField
{
  public:
    explicit UsernameField(std::optional<std::string> default_username = std::nullopt);
};
} // namespace user_forms
