#pragma once
#include "core/ui/form/form_context.hpp"

#include <any>
#include <functional>
#include <string>

namespace form
{
using ValueBinder = std::function<void(std::any&, const std::string&, const FormContext&)>;
}