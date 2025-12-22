#pragma once
#include <optional>
#include <string>
#include <vector>

namespace form {

class FormInputProvider {
 public:
  virtual ~FormInputProvider() = default;

  virtual std::string ReadText(const std::string& prompt) = 0;

  virtual std::optional<std::string> ReadSelection(const std::string& title,
                                                   const std::vector<std::string>& options) = 0;
};

}  // namespace form
