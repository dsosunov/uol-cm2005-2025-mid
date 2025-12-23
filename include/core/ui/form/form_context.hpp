#pragma once
#include <map>
#include <optional>
#include <string>

namespace form
{

  class FormContext
  {
  public:
    void SetValue(const std::string &key, const std::string &value);
    std::optional<std::string> GetValue(const std::string &key) const;
    void Clear();

  private:
    std::map<std::string, std::string, std::less<>> values_;
  };

} // namespace form
