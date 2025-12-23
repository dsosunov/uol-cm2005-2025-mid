#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/data_source.hpp"
#include "core/ui/form/form_context.hpp"

namespace form
{

    class ContextualDataSource : public DataSource
    {
    public:
        using ProviderFunc = std::function<std::vector<std::string>(const FormContext &)>;

        explicit ContextualDataSource(ProviderFunc provider);
        std::vector<std::string> GetOptions(const FormContext &context) const override;

    private:
        ProviderFunc provider_;
    };

} // namespace form
