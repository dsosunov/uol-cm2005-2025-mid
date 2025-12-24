#pragma once
#include <any>
#include <map>
#include <optional>
#include <string>
namespace form
{
class FormContext
{
  public:
    void SetValue(std::string_view key, std::any value);
    std::optional<std::any> GetValue(std::string_view key) const;

    // Template overload for type-safe retrieval with automatic casting
    template <typename T> std::optional<T> GetValue(std::string_view key) const
    {
        auto value = GetValue(key);
        if (value.has_value())
        {
            try
            {
                return std::any_cast<T>(*value);
            }
            catch (const std::bad_any_cast&)
            {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    void Clear();

  private:
    std::map<std::string, std::any, std::less<>> values_;
};
} // namespace form