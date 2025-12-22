#pragma once
#include <string>

#include "core/ui/form/field_validator.hpp"
#include "core/ui/form/fields/field.hpp"

namespace form {

class TextField : public Field {
 public:
  TextField(std::string name, std::string prompt, ValueBinder binder,
            FieldValidator validator = nullptr);

  const std::string& GetName() const override;
  const std::string& GetPrompt() const override;
  std::optional<std::string> ReadInput(FormInputProvider& input_provider,
                                       const FormContext& context) const override;
  ValidationResult Validate(const std::string& value, const FormContext& context) const override;
  void BindValue(std::any& target, const std::string& value,
                 const FormContext& context) const override;

 private:
  std::string name_;
  std::string prompt_;
  ValueBinder binder_;
  FieldValidator validator_;
};

}  // namespace form
