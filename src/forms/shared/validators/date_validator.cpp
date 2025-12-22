#include "forms/shared/validators/date_validator.hpp"

#include <cctype>

namespace forms::shared {

form::ValidationResult DateValidator::operator()(const std::string& value,
                                                 const form::FormContext& context) const {
  if (value.empty()) {
    return form::ValidationResult::Invalid("Date cannot be empty");
  }

  size_t first_slash = value.find('/');
  if (first_slash != std::string::npos) {
    size_t second_slash = value.find('/', first_slash + 1);
    if (second_slash == std::string::npos) {
      return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY");
    }

    std::string day_str = value.substr(0, first_slash);
    std::string month_str = value.substr(first_slash + 1, second_slash - first_slash - 1);
    std::string year_str = value.substr(second_slash + 1);

    if (day_str.empty() || day_str.length() > 2) {
      return form::ValidationResult::Invalid("Day must be 1-2 digits");
    }
    for (char c : day_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Day must be numeric");
      }
    }
    int day = std::stoi(day_str);
    if (day < 1 || day > 31) {
      return form::ValidationResult::Invalid("Day must be between 1 and 31");
    }

    if (month_str.empty() || month_str.length() > 2) {
      return form::ValidationResult::Invalid("Month must be 1-2 digits");
    }
    for (char c : month_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Month must be numeric");
      }
    }
    int month = std::stoi(month_str);
    if (month < 1 || month > 12) {
      return form::ValidationResult::Invalid("Month must be between 1 and 12");
    }

    if (year_str.length() != 4) {
      return form::ValidationResult::Invalid("Year must be 4 digits");
    }
    for (char c : year_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Year must be numeric");
      }
    }
    int year = std::stoi(year_str);
    if (year < 1900 || year > 2100) {
      return form::ValidationResult::Invalid("Year must be between 1900 and 2100");
    }

    return form::ValidationResult::Valid();
  }

  size_t first_dash = value.find('-');
  if (first_dash != std::string::npos) {
    size_t second_dash = value.find('-', first_dash + 1);
    if (second_dash == std::string::npos) {
      return form::ValidationResult::Invalid("Invalid date format. Expected YYYY-MM-DD");
    }

    std::string year_str = value.substr(0, first_dash);
    std::string month_str = value.substr(first_dash + 1, second_dash - first_slash - 1);
    std::string day_str = value.substr(second_dash + 1);

    if (year_str.length() != 4) {
      return form::ValidationResult::Invalid("Year must be 4 digits");
    }
    for (char c : year_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Year must be numeric");
      }
    }
    int year = std::stoi(year_str);
    if (year < 1900 || year > 2100) {
      return form::ValidationResult::Invalid("Year must be between 1900 and 2100");
    }

    if (month_str.empty() || month_str.length() > 2) {
      return form::ValidationResult::Invalid("Month must be 1-2 digits");
    }
    for (char c : month_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Month must be numeric");
      }
    }
    int month = std::stoi(month_str);
    if (month < 1 || month > 12) {
      return form::ValidationResult::Invalid("Month must be between 1 and 12");
    }

    if (day_str.empty() || day_str.length() > 2) {
      return form::ValidationResult::Invalid("Day must be 1-2 digits");
    }
    for (char c : day_str) {
      if (!std::isdigit(c)) {
        return form::ValidationResult::Invalid("Day must be numeric");
      }
    }
    int day = std::stoi(day_str);
    if (day < 1 || day > 31) {
      return form::ValidationResult::Invalid("Day must be between 1 and 31");
    }

    return form::ValidationResult::Valid();
  }

  return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY or YYYY-MM-DD");
}

}  // namespace forms::shared
