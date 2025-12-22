#pragma once
#include <string>
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/field_validator.hpp"

namespace form
{

class TextField : public Field {
 public:
  TextField(std::string name, std::string prompt, ValueBinder binder);

  const std::string& GetName() const override;
  const std::string& GetPrompt() const override;
  bool IsOptional() const override;
  ValidationResult Validate(const std::string& value,
                           const FormContext& context) const override;
  std::vector<std::string> GetOptions(const FormContext& context) const override;
  void BindValue(std::any& target, const std::string& value,
                const FormContext& context) const override;
  bool HasOptions() const override;

  void SetOptional(bool optional) override;
  void SetValidator(FieldValidator validator);

 private:
  std::string name_;
  std::string prompt_;
  ValueBinder binder_;
  FieldValidator validator_;
  bool is_optional_;
};

}  // namespace form
