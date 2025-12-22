#include "forms/user/validators/user_validators.hpp"

#include <cctype>
#include <regex>

namespace user_forms {

form::ValidationResult EmailValidator::operator()(const std::string& value,
                                                  const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Email cannot be empty");
  }

  size_t at_pos = value.find('@');
  if (at_pos == std::string::npos || at_pos == 0 || at_pos == value.length() - 1) {
    return form::ValidationResult::Invalid("Invalid email format");
  }

  size_t dot_pos = value.find('.', at_pos);
  if (dot_pos == std::string::npos || dot_pos == value.length() - 1) {
    return form::ValidationResult::Invalid("Invalid email format");
  }

  return form::ValidationResult::Valid();
}

form::ValidationResult UsernameValidator::operator()(const std::string& value,
                                                     const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Username cannot be empty");
  }

  if (value.length() < 3) {
    return form::ValidationResult::Invalid("Username must be at least 3 characters");
  }

  if (value.length() > 20) {
    return form::ValidationResult::Invalid("Username must be at most 20 characters");
  }

  for (char c : value) {
    if (!std::isalnum(c) && c != '_') {
      return form::ValidationResult::Invalid(
          "Username can only contain letters, numbers, and underscores");
    }
  }

  return form::ValidationResult::Valid();
}

form::ValidationResult PasswordValidator::operator()(const std::string& value,
                                                     const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Password cannot be empty");
  }

  if (value.length() < 6) {
    return form::ValidationResult::Invalid("Password must be at least 6 characters");
  }

  if (value.length() > 50) {
    return form::ValidationResult::Invalid("Password must be at most 50 characters");
  }

  return form::ValidationResult::Valid();
}

form::ValidationResult FullNameValidator::operator()(const std::string& value,
                                                     const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Full name cannot be empty");
  }

  if (value.length() < 2) {
    return form::ValidationResult::Invalid("Full name must be at least 2 characters");
  }

  if (value.length() > 100) {
    return form::ValidationResult::Invalid("Full name must be at most 100 characters");
  }

  bool has_letter = false;
  for (char c : value) {
    if (std::isalpha(c)) {
      has_letter = true;
      break;
    }
  }

  if (!has_letter) {
    return form::ValidationResult::Invalid("Full name must contain at least one letter");
  }

  return form::ValidationResult::Valid();
}

form::ValidationResult EmailOrUsernameValidator::operator()(
    const std::string& value, const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Email or username cannot be empty");
  }

  if (value.find('@') != std::string::npos) {
    EmailValidator email_validator;
    return email_validator(value, context);
  } else {
    UsernameValidator username_validator;
    return username_validator(value, context);
  }
}

}  // namespace user_forms
