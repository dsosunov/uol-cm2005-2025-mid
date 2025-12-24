#pragma once
#include "core/ui/form/data_source.hpp"
#include "core/ui/form/form_context.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace form
{
class StaticDataSource : public DataSource
{
  public:
    explicit StaticDataSource(std::vector<OptionPair> options);
    std::vector<OptionPair> GetOptions(const FormContext& context) const override;

  private:
    std::vector<OptionPair> options_;
};
} // namespace form