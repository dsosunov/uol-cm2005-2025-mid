#pragma once
#include <any>
#include <string>
#include <vector>
#include "core/ui/form/validation_result.hpp"
#include "core/ui/form/form_context.hpp"

namespace form
{

class Field {
 public:
  virtual ~Field() = default;

  virtual const std::string& GetName() const = 0;
  virtual const std::string& GetPrompt() const = 0;
  virtual bool IsOptional() const = 0;
  virtual ValidationResult Validate(const std::string& value,
                                   const FormContext& context) const = 0;
  virtual std::vector<std::string> GetOptions(const FormContext& context) const = 0;
  virtual void BindValue(std::any& target, const std::string& value,
                        const FormContext& context) const = 0;
  virtual bool HasOptions() const = 0;

  virtual void SetOptional(bool optional) = 0;
};

}  // namespace form
