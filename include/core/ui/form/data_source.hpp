#pragma once
#include "core/ui/form/form_context.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace form
{
class DataSource
{
  public:
    using OptionPair = std::pair<std::string, std::string>;
    virtual ~DataSource() = default;
    virtual std::vector<OptionPair> GetOptions(const FormContext& context) const = 0;
};
} // namespace form