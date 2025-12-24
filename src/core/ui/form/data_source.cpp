#include "core/ui/form/contextual_data_source.hpp"
namespace form
{
ContextualDataSource::ContextualDataSource(ProviderFunc provider) : provider_(std::move(provider))
{
}
std::vector<DataSource::OptionPair> ContextualDataSource::GetOptions(
    const FormContext& context) const
{
    return provider_(context);
}
} // namespace form