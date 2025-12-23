#pragma once
#include <map>
#include <optional>
#include <string>

namespace form
{

  class FormContext
  {
  public:
    void SetValue(std::string_view key, std::string_view value);
    std::optional<std::string> GetValue(std::string_view key) const;
    void Clear();

  private:
    std::map<std::string, std::string, std::less<>> values_;
  };

} // namespace form
