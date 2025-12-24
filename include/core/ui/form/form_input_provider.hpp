#pragma once
#include <any>
#include <optional>
#include <string>
namespace form
{
class Field;
class FormContext;
class FormInputProvider
{
  public:
    virtual ~FormInputProvider() = default;
    virtual std::optional<std::any> ReadField(const Field& field, const FormContext& context) = 0;
};
} // namespace form