#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/data_source.hpp"
#include "core/ui/form/field_validator.hpp"
#include "core/ui/form/fields/field.hpp"

namespace form
{

  class SelectionField : public Field
  {
  public:
    SelectionField(std::string name, std::string prompt, std::vector<std::string> options,
                   ValueBinder binder);

    SelectionField(std::string name, std::string prompt, std::shared_ptr<DataSource> data_source,
                   ValueBinder binder);

    const std::string &GetName() const override;
    const std::string &GetPrompt() const override;
    std::optional<std::string> GetValidationHint() const override;
    ValidationResult Validate(const std::string &value, const FormContext &context) const override;
    void BindValue(std::any &target, const std::string &value,
                   const FormContext &context) const override;

    /// Provider needs access to options for rendering menu
    /// Context is needed to evaluate dynamic data sources
    std::vector<std::string> GetOptions(const FormContext &context) const;

  private:
    std::string name_;
    std::string prompt_;
    std::vector<std::string> static_options_;
    std::shared_ptr<DataSource> data_source_;
    ValueBinder binder_;
  };

} // namespace form
