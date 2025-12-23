#include "core/ui/form/static_data_source.hpp"

namespace form
{

    StaticDataSource::StaticDataSource(std::vector<OptionPair> options)
        : options_(std::move(options)) {}

    std::vector<DataSource::OptionPair> StaticDataSource::GetOptions(const FormContext &) const
    {
        return options_;
    }

}
