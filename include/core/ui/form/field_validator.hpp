#pragma once
#include <any>
#include <functional>
#include <string>

#include "core/ui/form/form_context.hpp"

namespace form
{

    using ValueBinder = std::function<void(std::any &, const std::string &, const FormContext &)>;

} // namespace form
