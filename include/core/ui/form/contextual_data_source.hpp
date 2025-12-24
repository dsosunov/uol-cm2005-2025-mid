#pragma once
#include "core/ui/form/data_source.hpp"
#include "core/ui/form/form_context.hpp"

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace form
{
class ContextualDataSource : public DataSource
{
  public:
    using ProviderFunc = std::function<std::vector<OptionPair>(const FormContext&)>;
    explicit ContextualDataSource(ProviderFunc provider);
    std::vector<OptionPair> GetOptions(const FormContext& context) const override;

  private:
    ProviderFunc provider_;
};
} // namespace form