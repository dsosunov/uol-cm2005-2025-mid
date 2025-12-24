#include "core/ui/form/form_context.hpp"
namespace form
{
void FormContext::SetValue(std::string_view key, std::any value)
{
    values_[std::string(key)] = std::move(value);
}

std::optional<std::any> FormContext::GetValue(std::string_view key) const
{
    if (auto it = values_.find(std::string(key)); it != values_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

void FormContext::Clear()
{
    values_.clear();
}
} // namespace form