#pragma once
#include <any>
#include <functional>
#include <string>
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/form_context.hpp"

namespace form
{

using FieldValidator = std::function<ValidationResult(const std::string&, const FormContext&)>;
using ValueBinder = std::function<void(std::any&, const std::string&, const FormContext&)>;

}  // namespace form
