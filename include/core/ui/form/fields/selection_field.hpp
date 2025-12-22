#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/field_validator.hpp"
#include "core/ui/form/data_source.hpp"

namespace form
{

class SelectionField : public Field
{
public:
  // Constructor for static options
  SelectionField(std::string name, std::string prompt,
                std::vector<std::string> options, ValueBinder binder);

  // Constructor for dynamic options
  SelectionField(std::string name, std::string prompt,
                std::shared_ptr<DataSource> data_source, ValueBinder binder);

  const std::string& GetName() const override;
  const std::string& GetPrompt() const override;
  std::optional<std::string> ReadInput(FormInputProvider& input_provider,
                                       const FormContext& context) const override;
  ValidationResult Validate(const std::string& value,
                           const FormContext& context) const override;
  void BindValue(std::any& target, const std::string& value,
                const FormContext& context) const override;

private:
  std::string name_;
  std::string prompt_;
  std::vector<std::string> static_options_;
  std::shared_ptr<DataSource> data_source_;
  ValueBinder binder_;
};

}  // namespace form
