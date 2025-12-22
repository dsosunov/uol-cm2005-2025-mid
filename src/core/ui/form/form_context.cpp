#include "core/ui/form/form_context.hpp"

namespace form
{

void FormContext::SetValue(const std::string& key, const std::string& value) {
  values_[key] = value;
}

std::optional<std::string> FormContext::GetValue(const std::string& key) const {
  auto it = values_.find(key);
  if (it != values_.end()) {
    return it->second;
  }
  return std::nullopt;
}

void FormContext::Clear() {
  values_.clear();
}

}  // namespace form
