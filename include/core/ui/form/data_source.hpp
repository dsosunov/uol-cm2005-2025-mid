#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/form_context.hpp"

namespace form
{

  class DataSource
  {
  public:
    virtual ~DataSource() = default;
    virtual std::vector<std::string> GetOptions(const FormContext &context) const = 0;
  };

} // namespace form
