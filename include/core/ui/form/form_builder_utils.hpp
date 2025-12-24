#pragma once
#include "core/ui/form/fields/field.hpp"

#include <memory>
#include <vector>

namespace form
{
template <typename TField, typename... Args>
void AddField(std::vector<std::shared_ptr<Field>>& fields, Args&&... args)
{
    fields.push_back(std::make_shared<TField>(std::forward<Args>(args)...));
}
} // namespace form
