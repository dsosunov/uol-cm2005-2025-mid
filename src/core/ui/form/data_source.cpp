#include "core/ui/form/data_source.hpp"

namespace form
{

ContextualDataSource::ContextualDataSource(ProviderFunc provider)
    : provider_(std::move(provider)) {}

std::vector<std::string> ContextualDataSource::GetOptions(
    const FormContext& context) const {
  return provider_(context);
}

}  // namespace form
