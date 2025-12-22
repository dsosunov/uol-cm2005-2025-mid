#pragma once
#include <string>

namespace form {

struct ValidationResult {
  bool is_valid;
  std::string error_message;

  static ValidationResult Valid() { return {true, ""}; }

  static ValidationResult Invalid(std::string message) { return {false, std::move(message)}; }
};

}  // namespace form
