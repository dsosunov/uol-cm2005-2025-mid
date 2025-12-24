#pragma once
#include "core/ui/form/field_validator.hpp"
#include "core/ui/form/fields/field.hpp"
#include "core/ui/form/validator.hpp"

#include <memory>
#include <optional>
#include <string>

namespace form
{
class TextField : public Field
{
  public:
    TextField(std::string name, std::string prompt, ValueBinder binder,
              std::shared_ptr<Validator> validator = nullptr);
    const std::string& GetName() const override;
    const std::string& GetPrompt() const override;
    std::optional<std::string> GetValidationHint() const override;
    ValidationResult Validate(const std::string& value, const FormContext& context) const override;
    void BindValue(std::any& target, const std::string& value,
                   const FormContext& context) const override;

  private:
    std::string name_;
    std::string prompt_;
    ValueBinder binder_;
    std::shared_ptr<Validator> validator_;
};
} // namespace form