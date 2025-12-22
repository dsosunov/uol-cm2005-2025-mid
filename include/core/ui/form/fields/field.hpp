#pragma once
#include <any>
#include <optional>
#include <string>
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/form_context.hpp"

namespace form
{

class FormInputProvider;

class Field {
 public:
  virtual ~Field() = default;

  virtual const std::string& GetName() const = 0;
  virtual const std::string& GetPrompt() const = 0;

  /// Each field knows HOW to read its own input
  virtual std::optional<std::string> ReadInput(FormInputProvider& input_provider,
                                               const FormContext& context) const = 0;

  virtual ValidationResult Validate(const std::string& value,
                                   const FormContext& context) const = 0;
  virtual void BindValue(std::any& target, const std::string& value,
                        const FormContext& context) const = 0;
};

}  // namespace form
